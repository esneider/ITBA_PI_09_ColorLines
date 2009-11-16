// game.c
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "error.h"
#include "utils.h"
#include "playGame.h"
#include "game.h"



// creates a new game
game_t * newGame( options_t * options ){

	int i,j;
	game_t * sol = malloc( sizeof(game_t) );
	raiseErrorIf( sol, MEMORYERROR, NULL );

	sol->options = *options;

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

	sol->players = malloc( sol->numPlayers * sizeof(player_t) );
	if( !sol->players ){
		free( sol );
		raiseError( MEMORYERROR );
		return NULL;
	}

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
		sol->players[i].height = sol->options.height;
		sol->players[i].width = sol->options.width;
		randFill( sol, i, sol->options.initialTokens, true );
	}

	sol->state.next = 0;
	sol->state.lastTime = clock();
	sol->state.timeLeft = sol->options.initialSeconds * CLOCKS_PER_SEC;

	return sol;
}

void freeGame( game_t * game ){
	int i;
	for( i = 0 ; i <= game->numPlayers ; i++ ){
		freeMatrix( game->players[i].lastBoard.matrix, game->options.height );
		freeMatrix( game->players[i].board.matrix, game->options.height );
	}
	free( game->players );
	free( game );
}

void writeGame( game_t * game, char * file ){
	int i,x,y;
	int int_;
	char char_;
	FILE * out = fopen(file, "wb");
	raiseErrorIf(out,FILEERROR,);

	#define SAFE_FWRITE_INT( x )											\
		do{ int_ = (int)(x); 												\
			raiseErrorIf( fwrite( &int_, sizeof(int_), 1, out ), FILEERROR,);\
		}while(0)

	#define SAFE_FWRITE_CHAR( x )											\
		do{ char_ = (char)(x);												\
			raiseErrorIf( fwrite( &char_, sizeof(char_), 1, out ), FILEERROR,);\
		}while(0)

	SAFE_FWRITE_INT( game->options.mode );
	if( game->options.mode == TIMEMODE ){
		SAFE_FWRITE_INT( game->state.timeLeft / CLOCKS_PER_SEC );
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
				SAFE_FWRITE_CHAR( game->players[i].board.matrix[y][x] );
	}
	i = fclose(out);
	raiseErrorIf(i==0,FILEERROR,);

	#undef SAFE_FWRITE_INT
	#undef SAFE_FWRITE_CHAR
}

game_t * readGame(char * file){
	int i,x,y;
	options_t options;
	state_t state;
	game_t * sol;

	FILE * in  = fopen(file,"rb");
	raiseErrorIf(in,FILEERROR,NULL);

	#define SAFE_FREAD_INT( x )												\
		raiseErrorIf( fread( &(x), sizeof(int), 1, in ), FILEERROR, NULL )

	#define SAFE_FREAD_CHAR( x )											\
		raiseErrorIf( fread( &(x), sizeof(char), 1, in ), FILEERROR, NULL )

	SAFE_FREAD_INT( options.mode );
	if( options.mode == TIMEMODE ){
		SAFE_FREAD_INT( state.timeLeft );
		state.timeLeft *= CLOCKS_PER_SEC;
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

	sol = newGame( &options );
	sol->state = state;
	sol->state.lastTime = clock();

	for( i = 0 ; i < sol->numPlayers ; i++ ){
		SAFE_FREAD_INT( sol->players[i].board.points );
		sol->players[i].canUndo = false;
		sol->players[i].board.emptySpots = 0;
		sol->players[i].height = sol->options.height;
		sol->players[i].width = sol->options.width;
		for( y = 0 ; y < sol->options.height ; y++ )
			for( x = 0 ; x < sol->options.width ; x++ ){
				SAFE_FREAD_CHAR( sol->players[i].board.matrix[y][x] );
				sol->players[i].board.emptySpots += 
										sol->players[i].board.matrix[y][x] == 0;
			}
	}

	i = fclose(in);
	raiseErrorIf(i==0,FILEERROR,NULL);

	return sol;

	#undef SAFE_FREAD_INT
	#undef SAFE_FREAD_CHAR
}