// commands.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "utils.h"
#include "commands.h"

#define MAX_ARGS 10
#define MAX_COM_LEN 100

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

const command_t commands[]={
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
// 	printf("%s\n%d\n",s,argc);
	if( argc == 0 ) return true;

	sol = false;
	sprintf( err, "Unknown command");
	
	for( i = 0 ; i < sizeof(commands)/ sizeof(command_t) ; i++ )
		if( strncmp( argv[0], commands[i].com, strlen(commands[i].com) ) == 0 ){
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


bool movePiece( game_t * game, int argc, char ** argv, char * err ){
	int i;
	char s[ argc * MAX_ARGS ];

	s[0] = 0;
	for( i = 0 ; i < argc ; i++ )
		strcat( s, argv[i] );

	int x1, y1, x2, y2;
	i = sscanf( s, "[%d,%d][%d,%d]", &y1, &x1, &y2, &x2 );
	if( i < 4 ){
		sprintf( err, "Format error\n"
					"Must be: [ row_1, column_1 ][ row_2, column_2 ]" );
		return false;
	}
	if( ! entre( 0, y1, game->options.height ) ){
		sprintf( err, "Rank error\nThe first row must belong to the "
					"interval [0,%d]", game->options.height - 1 );
		return false;
	}
	if( ! entre( 0, x1, game->options.width ) ){
		sprintf( err, "Rank error\nThe first column must belong to the "
					"interval [0,%d]", game->options.width - 1 );
		return false;
	}
	if( ! entre( 0, y2, game->options.height ) ){
		sprintf( err, "Rank error\nThe fsecond row must belong to the "
					"interval [0,%d]", game->options.height - 1 );
		return false;
	}
	if( ! entre( 0, x2, game->options.width ) ){
		sprintf( err, "Rank error\nThe second column must belong to the "
					"interval [0,%d]", game->options.width - 1 );
		return false;
	}
	if( game->players[ game->state.next ].board.board[y1][x1] == 0 ){
		sprintf( err, "The origin position must not be empty" );
		return false;
	}
	if( game->players[ game->state.next ].board.board[y2][x2] != 0 ){
		sprintf( err, "The target position must not be occupied" );
		return false;
	}

	// 	TODO
	// 	BFS para encontrar el camino minimo
	
	return true;
}

bool save( game_t * game, int argc, char ** argv, char * err ){

	if( argc == 1 ){
		sprintf( err, "Missing file operand\n"
					"Try 'save --help' for more information.");
		return false;
	}
	if( argc > 2 ){
		sprintf( err, "Wrong usage\n"
					"Try 'save --help' for more information.");
		return false;	
	}
	if( strcmp( argv[1], "--help" ) == 0 ){
		sprintf( err, "Usage: save FILE\n\n"
					"Saves the current game to file FILE");
		return false;
	}

	writeGame( game, argv[1] );

	return true;
}

bool buyItem( game_t * game, int argc, char ** argv, char * err ){
	return true;
}

bool throwItem( game_t  * game, int argc, char ** argv, char * err ){
	return true;
}

bool undo( game_t * game, int argc, char ** argv, char * err ){
// 	if( ->mode > 1 ){
// 		sprintf( err, "Comando undo no disponible para modo multijugador" );
// 		return false;
// 	}
// 	if( game->p2points == -1 ){
// 		sprintf( err, "El comando undo no se puede usar dos veces seguidas en "
// 						"un mismo turno, y despues de la primer jugada" );
// 		return false;
// 	}
// 
// 	game->p1points = game->p2points;
// 	game->p2points = -1;
// 	char ** aux = game->p1;
// 	game->p1 = game->p2;
// 	game->p2 = aux;

	return true;
}

bool quit( game_t * game, int argc, char ** argv, char * err ){
	return true;
}

bool help     ( game_t * game, int argc, char ** argv, char * err ){
	return true;
}
