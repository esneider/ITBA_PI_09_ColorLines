/**
* @file game.c
* operations for the struct @c game
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "error.h"
#include "utils.h"
#include "defines.h"
#include "playGame.h"
#include "game.h"


#define SAFE_FWRITE_INT( x )											\
	do{ int_ = (int)(x); 												\
		raiseErrorIf( fwrite( &int_, sizeof(int_), 1, out ), FILEERROR,);\
	}while(0)

#define SAFE_FWRITE_CHAR( x )											\
	do{ char_ = (char)(x);												\
		raiseErrorIf( fwrite( &char_, sizeof(char_), 1, out ), FILEERROR,);\
	}while(0)

#define SAFE_FREAD_INT( x )												\
	do{ raiseErrorIf( fread( &(x), sizeof(int), 1, in ),				\
					feof(in) ? CORRUPTFILE : FILEERROR, NULL ); }while(0)

#define SAFE_FREAD_CHAR( x )											\
	do{ raiseErrorIf( fread( &(x), sizeof(char), 1, in ),				\
					feof(in) ? CORRUPTFILE : FILEERROR, NULL ); }while(0)


/**
* Creates a new game, based on @a options.
*
* @throws MEMORYERROR			if there was a problem while allocating memory
* @throws COMPUTATIONALERROR	if after @b MAX_WAITING_TIME time no solution
*								for {@link randFill()} has been obtained
*
* @param options	contains options about the game
*
* @return a pointer to the new game
*
* @see newMatrix()
* @see randfill()
*/

game_t * newGame( options_t * options ){

	int i,j;
	game_t * sol = malloc( sizeof(game_t) );
	raiseErrorIf( sol, MEMORYERROR, NULL );

	sol->options = *options;
	// filling the game mode
	switch( sol->options.mode ){
		case SINGLEMODE:
		case TIMEMODE:
			sol->numPlayers = 1;
			break;
		case MULTIPLMODE:
			sol->numPlayers = 2;
			break;
		default:
			sol->numPlayers = 0;
			break;
	};
	// filling players
	sol->players = malloc( sol->numPlayers * sizeof(player_t) );
	if( !sol->players ){
		free( sol );
		raiseError( MEMORYERROR );
		return NULL;
	}
	// filling the players boards
	for( i = 0 ; i < sol->numPlayers ; i++ ){
		sol->players[i].board.matrix=
			newMatrix( sol->options.height, sol->options.width );
		sol->players[i].lastBoard.matrix =
			newMatrix( sol->options.height, sol->options.width );

		if( !sol->players[i].board.matrix || !sol->players[i].lastBoard.matrix ){
			for( j = 0 ; j <= i ; j++ ){
				freeMatrix( sol->players[j].lastBoard.matrix, sol->options.height );
				freeMatrix( sol->players[j].board.matrix, sol->options.height );
			}
			free( sol->players );
			free( sol );
			raiseError( MEMORYERROR );
			return NULL;
		}

		sol->players[i].board.points = 0;
		sol->players[i].board.emptySpots = sol->options.height * sol->options.width;
		sol->players[i].canUndo = false;

		randFill( sol, i, sol->options.initialTokens, true );

		if( errorCode() != NOERROR ){
			for( j = 0 ; j <= i ; j++ ){
				freeMatrix( sol->players[j].lastBoard.matrix, sol->options.height );
				freeMatrix( sol->players[j].board.matrix, sol->options.height );
			}
			free( sol->players );
			free( sol );
			return NULL;
		}
	}
	// filling the starting conditions
	sol->state.next = 0;
	sol->state.lastTime = time(NULL);
	sol->state.timeLeft = sol->options.initialSeconds;

	return sol;
}


/**
* Frees the memory reserved by {@link newGame()}.
*
* @param  game	the information of the game about to be freed
*
* @see newGame()
* @see freeMatrix()
*/

void freeGame( game_t * game ){
	int i;
	for( i = 0 ; i < game->numPlayers ; i++ ){
		freeMatrix( game->players[i].lastBoard.matrix, game->options.height );
		freeMatrix( game->players[i].board.matrix, game->options.height );
	}
	free( game->players );
	free( game );
}


/**
* Saves @a game in a file.
*
* @throws FILEERROR	if there was an problem while opening/writing the file
*
* @param  game	contains all the data about the game
* @param  file	contains the name of file about to be saved
*
* @see readGame()
*/

void writeGame( game_t * game, char * file ){
	int i,x,y;
	int int_;
	char char_;
	//opening the file
	FILE * out = fopen(file, "wb");
	raiseErrorIf(out,FILEERROR,);

	// saving all the content in the file
	SAFE_FWRITE_INT( game->options.mode );
	if( game->options.mode == TIMEMODE ){
		time_t aux = time(NULL);
		game->state.timeLeft -= aux - game->state.lastTime;
		game->state.lastTime = aux;
		SAFE_FWRITE_INT( game->state.timeLeft );
	}else
	if( game->options.mode == MULTIPLMODE )
		SAFE_FWRITE_INT( game->state.next + 1 );
	SAFE_FWRITE_INT( game->options.height );
	SAFE_FWRITE_INT( game->options.width );
	SAFE_FWRITE_INT( game->options.numColors );
	SAFE_FWRITE_INT( game->options.tokensPerLine );
	SAFE_FWRITE_INT( game->options.tokensPerTurn );
	for( i = 0 ; i < game->numPlayers ; i++ ){
		SAFE_FWRITE_INT( game->players[i].board.points );
		for( y = 0 ; y < game->options.height ; y++ )
			for( x = 0 ; x < game->options.width ; x++ )
				SAFE_FWRITE_CHAR( game->players[i].board.matrix[y][x] + '0' );
	}
	//finishing and closing the file
	i = fclose(out);
	raiseErrorIf(i==0,FILEERROR,);
}


/**
* Validates @a game, in order to prevent corrupted input files.
*
* @param  game	contains all the data about the game
*
* @return true if the game is valid, otherwise, false
*/

static bool validateGame( game_t * game ){
	int i,x,y;

	if( !entre( 0, game->options.mode, 3 ) )
		return false;
	if( game->options.mode == TIMEMODE &&
						!entre( 0, game->state.timeLeft, 60*MAX_MINUTES + 1 ) )
		return false;
	if( game->options.mode == MULTIPLMODE && !entre( 0, game->state.next, 2 ) )
		return false;
	else
	if( !entre( 0, game->state.next, 1 ) )
		return false;
	if( !entre( MIN_TAB_DIM, game->options.width, MAX_TAB_DIM + 1) )
		return false;
	if( !entre( MIN_TAB_DIM, game->options.height, MAX_TAB_DIM + 1 ) )
		return false;
	if( !entre( MIN_COLORS, game->options.numColors, MAX_COLORS + 1 ) )
		return false;
	if( !entre( MIN_TOK_PER_LINE, game->options.tokensPerLine,
						min( game->options.width, game->options.height ) + 1 ) )
		return false;
	if( !entre( 1, game->options.tokensPerTurn,
							game->options.width * game->options.height + 1 ) )
		return false;
	for( i = 0 ; i < game->numPlayers ; i++ ){
		if( game->players[i].board.points < 0 )
			return false;
		for( y = 0 ; y < game->options.height ; y++ )
			for( x = 0 ; x < game->options.width ; x++ )
				if( !entre( 0, game->players[i].board.matrix[y][x],
													game->options.numColors +1))
					return false;
	}
	return true;
}


/**
* Reads a game from a file.  Use {@link newGame()} to load the game.
*
* @throws MEMORYERROR	if there was an problem while allocating memory
* @throws FILEERROR		if there was an problem while opening/reading from file
*
* @param file	contains the name of the file about to be read
*
* @return a pointer to a game_t containing all the data about the game
*
* @see writeGame()
* @see newgame()
*/

game_t * readGame(char * file){
	int i,x,y;
	options_t options;
	state_t state;
	game_t * sol;

	FILE * in  = fopen(file,"rb");
	raiseErrorIf(in,FILEERROR,NULL);

	SAFE_FREAD_INT( options.mode );
	state.next = 0;
	if( options.mode == TIMEMODE ){
		SAFE_FREAD_INT( state.timeLeft );
		state.lastTime = time(NULL);
	} else
	if( options.mode == MULTIPLMODE ){
		SAFE_FREAD_INT( state.next );
		state.next--;
	}
	SAFE_FREAD_INT( options.height );
	SAFE_FREAD_INT( options.width );
	SAFE_FREAD_INT( options.numColors );
	SAFE_FREAD_INT( options.tokensPerLine );
	SAFE_FREAD_INT( options.tokensPerTurn );

	options.initialTokens = 0;
	sol = newGame( &options );
	raiseErrorIf(errorCode()==NOERROR,errorCode(),NULL);
	sol->state = state;

	for( i = 0 ; i < sol->numPlayers ; i++ ){
		SAFE_FREAD_INT( sol->players[i].board.points );
		for( y = 0 ; y < sol->options.height ; y++ )
			for( x = 0 ; x < sol->options.width ; x++ ){
				SAFE_FREAD_CHAR( sol->players[i].board.matrix[y][x] );
				sol->players[i].board.matrix[y][x] -= '0';
				if( sol->players[i].board.matrix[y][x] )
					sol->players[i].board.emptySpots--;
			}
	}
	raiseErrorIf(!fread( &i, sizeof(int), 1, in ),CORRUPTFILE,NULL);
	i = fclose(in);
	raiseErrorIf(i==0,FILEERROR,NULL);

	raiseErrorIf(validateGame(sol),CORRUPTFILE,NULL);

	return sol;
}