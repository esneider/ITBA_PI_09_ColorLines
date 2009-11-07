// commands.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "mytime.h"
#include "commands.h"

#define MAX_COMMAND_LENGTH 100

// b \in [a,b)   WARNING: evalua b 2 veces!!
#define entre(a,b,c) ((a)<=(b)&&(b)<(c))

bool move_piece( game * G, int x1, int y1, int x2, int y2, char * err ){
	if( ! entre( 0, y1, G->height ) ){
		sprintf( err, "Error de rango.\nLa primer fila debe pertenecer al intervalo [0,%d]", G->height-1 );
		return false;
	}
	if( ! entre( 0, x1, G->width ) ){
		sprintf( err, "Error de rango.\nLa primer columna debe pertenecer al intervalo [0,%d]", G->width-1 );
		return false;
	}
	if( ! entre( 0, y2, G->height ) ){
		sprintf( err, "Error de rango.\nLa segunda fila debe pertenecer al intervalo [0,%d]", G->height-1 );
		return false;
	}
	if( ! entre( 0, x2, G->width ) ){
		sprintf( err, "Error de rango.\nLa segunda columna debe pertenecer al intervalo [0,%d]", G->width-1 );
		return false;
	}
	
	char ** pl;
	int * plpoints;
	
	if( G->mode == 2 && G->un.next == 2 ){
		// player 2 turn
		pl = G->p2;
		plpoints = &( G->p2point );
	}else{
		// player 1 turn
		pl = G->p1;
		plpoints = &( G->p1point );
	}
	
	if( pl[y1][x1] == 0 ){
		sprintf( err, "La posicion de origen no debe estar vacia" );
		return false;
	}
	if( pl[y2][x2] != 0 ){
		sprintf( err, "La posicion de destino debe estar vacia" );
		return false;
	}
	// TODO
	// BFS para encontrar el camino minimo
	// bool connected = bfs()
	#undef p1
	#undef p1points
	return true;
}

bool save( game * G, char * s2, char * err ){
	if( G->mode == 1 ){
		G->un.time = time_left(G);
		reset_time();
	}

	clear_error();
	write_game( G, s2 );
	err = error_message( error_code() );

	return error_code() == NOERROR;
}

bool buy_item( game * G, char * s2, char * err ){
	return true;
}

bool throw_item( game * G, char * s2, char * err ){
	return true;
}

bool undo( game * G, char * err ){
	if( G->mode > 1 ){
		sprintf( err, "Comando undo no disponible para modo multijugador" );
		return false;
	}
	if( G->p2points == -1 ){
		sprintf( err, "El comando undo no se puede usar dos veces seguidas en un mismo turno, y despues de la primer jugada" );
		return false;
	}

	G->p1points = G->p2points;
	G->p2points = -1;
	char ** aux = G->p1;
	G->p1 = G->p2;
	G->p2 = aux;

	return true;
}

bool quit( game * G, char * err ){
	return true;
}


bool new_command( game * G, char * s, char * err ){
	int x1, y1, x2, y2;
	int args;
	char s1[ MAX_COMMAND_LENGTH ], s2[ MAX_COMMAND_LENGTH ];

// COORDS
	args = sscanf( s, "[%d,%d][%d,%d]", &y1, &x1, &y2, &x2 );
	if( args > 0 ){
		if( args < 4 ){
			sprintf( err, "Error en formato.\nDebe ser \"[ fila_1, columna_1 ][ fila_2, columna_2 ]\"" );
			return false;
		}
		return move_piece( G, x1, y1, x2, y2, err );
	}

// OTHERS

	args = sscanf( s, "%s %s", s1, s2 );

// two string commands
	if( args > 1 && s2[0]!='\n' && s2[0]!='\r' ){

// SAVE
		if( strcmp( s1, "save" ) == 0 ){
			return save( G, s2, err );
		}
// BUY
		if( strcmp( s1, "buy" ) == 0 )
			return buy_item( G, s2, err );
// THROW
		if( strcmp( s1, "throw" ) == 0 )
			return throw_item( G, s2, err );
	}

// one string commands
	if( args == 1 && s1[0]!='\n' && s1[0]!='\r' ){

// UNDO
		if( strcmp( s1, "undo" ) == 0 )
			return undo( G, err );
// QUIT
		if( strcmp( s1, "quit" ) == 0 )
			return quit( G, err );
// more commands
	}
	
	sprintf( err, "Comando desconocido");
	return false;
}
