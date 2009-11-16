// commands.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "utils.h"
#include "defines.h"
#include "playGame.h"
#include "commands.h"

bool movePiece( game_t * game, int argc, char ** argv, char * err );
bool save     ( game_t * game, int argc, char ** argv, char * err );
bool buyItem  ( game_t * game, int argc, char ** argv, char * err );
bool throwItem( game_t * game, int argc, char ** argv, char * err );
bool undo     ( game_t * game, int argc, char ** argv, char * err );
bool quit     ( game_t * game, int argc, char ** argv, char * err );
bool help     ( game_t * game, int argc, char ** argv, char * err );

typedef struct{
	char com[ MAX_COM_LEN ];
	bool (* func )( game_t *, int, char **, char * );
} command_t;

const command_t commands[] = {
	{"[", movePiece},
	{"save", save},
	{"buy", buyItem},
	{"throw", throwItem},
	{"undo", undo},
	{"quit", quit},
	{"help", help}
};


bool newCommand( game_t * game, char * s, char * err ){
	int i;
	bool sol;
	int argc;
	char ** argv;

	clearError();

	argv = newMatrix( MAX_ARGS, MAX_COM_LEN );

	if( errorCode() != NOERROR ){
		err = errorMessage( errorCode() );
		return false;
	}

	argc = sscanf( s, "%s %s %s %s %s %s %s %s %s %s",
				   argv[0], argv[1], argv[2], argv[3], argv[4],
				   argv[5], argv[6], argv[7], argv[8], argv[9] );

	if( argc < 1  ) return true;

	sol = false;
	sprintf( err, "Unknown command");

	for( i = 0 ; i < sizeof(commands)/ sizeof(command_t) ; i++ )

		if( strncmp( argv[0], commands[i].com, strlen(commands[i].com) ) == 0 ){
			err[0] = 0;
			sol = commands[i].func( game, argc, argv, err );

			if( errorCode() != NOERROR ){
				err = errorMessage( errorCode() );
				sol = false;
			}
			break;
		}

	freeMatrix( argv, MAX_ARGS );
	return sol;
}

/** *******************************************************************
****************************** COMMANDS *******************************
******************************************************************* **/

/**********************************************************************
****************************** movePiece ******************************
**********************************************************************/

bool movePiece( game_t * game, int argc, char ** argv, char * err ){
	int i;
	char s[ argc * MAX_ARGS ];

	s[0] = 0;
	for( i = 0 ; i < argc ; i++ )
		strcat( s, argv[i] );

	int x1, y1, x2, y2;
	i = sscanf( s, "[%d,%d][%d,%d]", &y1, &x1, &y2, &x2 );
	if( i < 4 ){
		sprintf( err, "Format error:\n"
					"Must be: [ row_1, column_1 ][ row_2, column_2 ]" );
		return false;
	}
	if( ! entre( 0, y1, game->options.height ) ){
		sprintf( err, "Rank error:\nThe first row must belong to the "
					"interval [0,%d]", game->options.height - 1 );
		return false;
	}
	if( ! entre( 0, x1, game->options.width ) ){
		sprintf( err, "Rank error:\nThe first column must belong to the "
					"interval [0,%d]", game->options.width - 1 );
		return false;
	}
	if( ! entre( 0, y2, game->options.height ) ){
		sprintf( err, "Rank error:\nThe second row must belong to the "
					"interval [0,%d]", game->options.height - 1 );
		return false;
	}
	if( ! entre( 0, x2, game->options.width ) ){
		sprintf( err, "Rank error:\nThe second column must belong to the "
					"interval [0,%d]", game->options.width - 1 );
		return false;
	}
	if( game->players[ game->state.next ].board.matrix[y1][x1] == 0 ){
		sprintf( err, "The origin position must not be empty" );
		return false;
	}
	if( game->players[ game->state.next ].board.matrix[y2][x2] != 0 ){
		sprintf( err, "The target position must not be occupied" );
		return false;
	}

	// BFS para encontrar el camino minimo

	struct coord{
		int x,y;
	} move[4] = { {-1,0}, {0,1}, {1,0}, {0,-1} };

	struct node{
		int x, y, parent;
	} queue[ game->players[ game->state.next ].board.emptySpots + 1 ];

	int read = -1, write = 0,x,y;

	bool touched[ game->options.height ][ game->options.width ];

	memset( &touched[0][0], false, sizeof(touched)/sizeof(bool) );
	queue[write++] = (struct node){x1,y1,-1};

	while( ++read < write ){
		if( queue[read].x == x2 && queue[read].y == y2 )
			break;
		for( i = 0 ; i < sizeof(move)/sizeof(struct coord) ; i++ ){
			x = queue[read].x + move[i].x;
			y = queue[read].y + move[i].y;
			if( entre(0,x,game->options.width)
				&& entre(0,y,game->options.height)
				&& game->players[ game->state.next ].board.matrix[y][x] == 0
				&& !touched[y][x] ){
					touched[y][x] = true;
					queue[write++] = (struct node){x,y,read};
			}
		}
	}

	if( read >= write ){
		sprintf( err, "There must be a path of unoccupied spaces from the "
					"origin position to the target position" );
		return false;
	}

	game->players[ game->state.next ].canUndo = true;
	game->players[ game->state.next ].lastBoard =
										game->players[ game->state.next ].board;
	// contar puntos

	// draw movement TODO (needs front-end)
	if( ! notFree( game, game->state.next, x2, y2,
					game->players[ game->state.next ].board.matrix[y1][x1] ) ){

		game->players[ game->state.next ].board.matrix[y2][x2] =
						game->players[ game->state.next ].board.matrix[y1][x1];
		game->players[ game->state.next ].board.matrix[y1][x1] = 0;

		randFill( game, game->state.next, game->options.tokensPerTurn, false );
	}

	i = 0; 
	do{
		game->state.next++;
		game->state.next %= game->numPlayers;
		i++;
	}while( game->players[ game->state.next ].board.emptySpots <= 0 &&
				i <= game->numPlayers );
	if( i > game->numPlayers ){
		// GAME OVER FOR EVERYONE
	}

	return true;
}

/**********************************************************************
******************************** save *********************************
**********************************************************************/

bool save( game_t * game, int argc, char ** argv, char * err ){

	if( argc == 1 ){
		sprintf( err, "Missing file operand\n"
					"Try 'save --help' for more information");
		return false;
	}
	if( argc > 2 ){
		sprintf( err, "Wrong usage\n"
					"Try 'save --help' for more information");
		return false;	
	}
	if( strcmp( argv[1], "--help" ) == 0 ){
		sprintf( err, "Usage: save filename\n"
					"Saves the current game to file 'filename'");
		return false;
	}

	writeGame( game, argv[1] );

	return true;
}

/**********************************************************************
******************************* buyItem *******************************
**********************************************************************/

bool buyItem( game_t * game, int argc, char ** argv, char * err ){
	//TODO
	return true;
}

/**********************************************************************
****************************** throwItem ******************************
**********************************************************************/

bool throwItem( game_t  * game, int argc, char ** argv, char * err ){
	//TODO
	return true;
}

/**********************************************************************
********************************* undo ********************************
**********************************************************************/

bool undo( game_t * game, int argc, char ** argv, char * err ){
	if( argc == 2 && strcmp( argv[1], "--help" ) == 0 ){
		sprintf( err, "Usage: undo\n"
					"Undoes the last move\n"
					"It can only be used once some move has been done, "
					"and it can't be used two consecutive times" );
		return false;
	}
	if( argc > 1 ){
		sprintf( err, "Wrong usage\n"
					"Try 'undo --help' for more information");
		return false;
	}
	if( game->options.mode != SINGLEMODE ){
		sprintf( err, "'undo' command is only available in one player"
					", no time mode" );
		return false;
	}
	if( game->players[ game->state.next ].canUndo == false ){
		sprintf( err, "'undo' command cannot be used twice in a row or in "
					"the first turn" );
		return false;
	}

	game->players[ game->state.next ].canUndo = false;
	// free old matrix
	game->players[ game->state.next ].board =
									game->players[ game->state.next ].lastBoard;

	return true;
}

/**********************************************************************
********************************* quit ********************************
**********************************************************************/

bool quit( game_t * game, int argc, char ** argv, char * err ){
	if( argc == 2 && strcmp( argv[1], "--help" ) == 0 ){
		sprintf( err, "Usage: quit\n"
					"Quits the current game" );
		return false;
	}
	if( argc > 1){
		sprintf( err, "Wrong usage\n"
			"Try 'quit --help' for more information");
		return false;
	}

	//TODO
	err[0]=0;
	return false;
	//TODO
	return true;
}

/**********************************************************************
********************************* help ********************************
**********************************************************************/

bool help     ( game_t * game, int argc, char ** argv, char * err ){
	sprintf( err, //"These commands are defined internally\n"
				"Type 'name --help' to find out more about the function 'name'"
				"\n \n"
				"  [row_1,column_1][row_2,column_2]\n"
				"  save filename\n"
				"  undo\n"
				"  quit\n"
				"  help\n"
				"  buy {item}\n"
				"  throw {item}\n" );
	return true;
}
