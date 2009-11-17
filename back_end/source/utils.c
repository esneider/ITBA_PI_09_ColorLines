// utils.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "utils.h"

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

void freeMatrix( char ** mat, int height ){
	int i;
	for( i = 0 ; i < height ; i++ )
		free( mat[i] );
	free(mat);
}

void copyMatrix( char ** from, char ** to, int height, int width ){
	for( height-- ; height >= 0 ; height-- )
		memcpy( to[height], from[height], width );
}

bool entre( const int a, const int b, const int c ){
	return a<=b && b<c;
}

void clearBuffer(){
	while(getchar() != '\n');
}

bool validateInt( int a, int b, int c, char * err ){
	if( !entre(a,b,c) ){
		sprintf( err, "Rank error:\nIt must belong to the "
		"interval [%d,%d]", a, c-1 );
		return false;
	}
	return true;
}

int min( int a, int b ){
	return (a<=b)?a:b;
}