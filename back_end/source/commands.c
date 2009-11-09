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
	{"quit", quit}
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


bool movePiece( game_t * game, int argc, char ** argv, char * err ){return true;}
// bool movePiece( game_t * game, int x1, int y1, int x2, int y2, char * err ){
// 	// COORDS
// 	int x1, y1, x2, y2;
// 	argc = sscanf( s, "[%d,%d][%d,%d]", &y1, &x1, &y2, &x2 );
// 	if( argc > 0 ){
// 		if( argc < 4 ){
// 			sprintf( err, "Error en formato.\nDebe ser \"[ fila_1, columna_1 ]"
// 			"[ fila_2, columna_2 ]\"" );
// 			return false;
// 		}
// 		return movePiece( game, x1, y1, x2, y2, err );
// 	}
// 	if( ! entre( 0, y1, game->options.height ) ){
// 		sprintf( err, "Error de rango.\nLa primer fila debe pertenecer al "
// 						"intervalo [0,%d]", game->options.height-1 );
// 		return false;
// 	}
// 	if( ! entre( 0, x1, game->options.width ) ){
// 		sprintf( err, "Error de rango.\nLa primer columna debe pertenecer al "
// 						"intervalo [0,%d]", game->options.width-1 );
// 		return false;
// 	}
// 	if( ! entre( 0, y2, game->options.height ) ){
// 		sprintf( err, "Error de rango.\nLa segunda fila debe pertenecer al "
// 						"intervalo [0,%d]", game->options.height-1 );
// 		return false;
// 	}
// 	if( ! entre( 0, x2, game->options.width ) ){
// 		sprintf( err, "Error de rango.\nLa segunda columna debe pertenecer al "
// 						"intervalo [0,%d]", game->options.width-1 );
// 		return false;
// 	}
// 	
// /*	char ** pl;
// 	int * plpoints;
// 	
// 	if( game->options.mode == MULTIPLMODE && game->state.next == 1 ){
// 		player 2 turn
// 		pl = game->p2;
// 		plpoints = &( game->p2point );
// 	}else{
// 		player 1 turn
// 		pl = game->p1;
// 		plpoints = &( game->p1point );
// 	}*/
// 	
// /*	if( pl[y1][x1] == 0 ){
// 		sprintf( err, "La posicion de origen no debe estar vacia" );
// 		return false;
// 	}
// 	if( pl[y2][x2] != 0 ){
// 		sprintf( err, "La posicion de destino debe estar vacia" );
// 		return false;
// 	}*/
// // 	TODO
// // 	BFS para encontrar el camino minimo
// // 	bool connected = bfs()
// 	return true;
// }

bool save( game_t * game, int argc, char ** argv, char * err ){
/*	if( game->mode == 1 ){
		game->un.time = time_left(game);
		reset_time();
	}

	clearError();
	writeGame( game, s2 );
	err = errorMessage( error_code() );
*/
	return errorCode() == NOERROR;
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

