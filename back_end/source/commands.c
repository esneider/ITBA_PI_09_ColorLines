// commands.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "commands.h"

#define MAX_ARGS 10
#define MAX_COM_LEN 100

// b \in [a,b)   WARNING: evalua b 2 veces!!
#define entre(a,b,c) ((a)<=(b)&&(b)<(c))


void freeMatrix( char ** mat, int height ){
	int i;
	for( i = 0 ; i < height ; i++ )
		free( mat[i] );
	free(mat);
}

// creates a new matrix of height x width and 0es it
char ** newMatrix( int height, int width ){
	int i;
	char ** sol = malloc( height * sizeof(char*) );
	raiseErrorIf( sol, MEMORYERROR, NULL );
	
	for( i = 0 ; i < height ; i++ ){
		sol[i] = calloc( width, sizeof(char) );
		// free allocated memory in case of error
		if( !sol ){
			freeMatrix( sol, i-1 );
			raiseError( MEMORYERROR );
			return NULL;
		}
	}
	return sol;
}


bool movePiece( game_t * game, int x1, int y1, int x2, int y2, char * err ){
	if( ! entre( 0, y1, game->options.height ) ){
		sprintf( err, "Error de rango.\nLa primer fila debe pertenecer al intervalo [0,%d]", game->options.height-1 );
		return false;
	}
	if( ! entre( 0, x1, game->options.width ) ){
		sprintf( err, "Error de rango.\nLa primer columna debe pertenecer al intervalo [0,%d]", game->options.width-1 );
		return false;
	}
	if( ! entre( 0, y2, game->options.height ) ){
		sprintf( err, "Error de rango.\nLa segunda fila debe pertenecer al intervalo [0,%d]", game->options.height-1 );
		return false;
	}
	if( ! entre( 0, x2, game->options.width ) ){
		sprintf( err, "Error de rango.\nLa segunda columna debe pertenecer al intervalo [0,%d]", game->options.width-1 );
		return false;
	}
	
/*	char ** pl;
	int * plpoints;
	
	if( game->options.mode == MULTIPLMODE && game->state.next == 1 ){
		player 2 turn
		pl = game->p2;
		plpoints = &( game->p2point );
	}else{
		player 1 turn
		pl = game->p1;
		plpoints = &( game->p1point );
	}*/
	
/*	if( pl[y1][x1] == 0 ){
		sprintf( err, "La posicion de origen no debe estar vacia" );
		return false;
	}
	if( pl[y2][x2] != 0 ){
		sprintf( err, "La posicion de destino debe estar vacia" );
		return false;
	}*/
// 	TODO
// 	BFS para encontrar el camino minimo
// 	bool connected = bfs()
	return true;
}

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
// 		sprintf( err, "El comando undo no se puede usar dos veces seguidas en un mismo turno, y despues de la primer jugada" );
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


bool newCommand( game_t * game, char * s, char * err ){
	int argc;

// COORDS
	int x1, y1, x2, y2;
	argc = sscanf( s, "[%d,%d][%d,%d]", &y1, &x1, &y2, &x2 );
	if( argc > 0 ){
		if( argc < 4 ){
			sprintf( err, "Error en formato.\nDebe ser \"[ fila_1, columna_1 ][ fila_2, columna_2 ]\"" );
			return false;
		}
		return movePiece( game, x1, y1, x2, y2, err );
	}

// OTHERS

// 	char ** argv = newMatrix( MAX_ARGS, MAX_COM_LEN );

	char * argv[ MAX_ARGS ];
	char str[ MAX_ARGS * MAX_COM_LEN ];
	for( int i = 0 ; i < MAX_ARGS ; i++ )
		argv[i] = & str[ i * MAX_COM_LEN ];

	argc = sscanf( s, "%s %s %s %s %s %s %s %s %s %s",
				   argv[0], argv[1], argv[2], argv[3], argv[4],
				   argv[5], argv[6], argv[7], argv[8], argv[9] );

	if( argc == 0 ) return true;

// #define TEST_COMMAND( s, f ) if( strcmp( argv[0], s ) == 0 ) return f( game, argc, argv, err )
// 
// 	TEST_COMMAND("save",save);
// 	TEST_COMMAND("buy",buyitem);
// 	TEST_COMMAND("throw",throwItem);
// 	TEST_COMMAND("undo",undo);
// 	TEST_COMMAND("quit",quit);

// SAVE
	if( strcmp( argv[0], "save" ) == 0 )
		return save( game, argc, argv, err );
// BUY
	if( strcmp( argv[0], "buy" ) == 0 )
		return buyItem( game, argc, argv, err );
// THROW
	if( strcmp( argv[0], "throw" ) == 0 )
		return throwItem( game, argc, argv, err );
// UNDO
	if( strcmp( argv[0], "undo" ) == 0 )
		return undo( game, argc, argv, err );
// QUIT
	if( strcmp( argv[0], "quit" ) == 0 )
		return quit( game, argc, argv, err );
// more commands


	sprintf( err, "Unknown command");
	return false;
}
