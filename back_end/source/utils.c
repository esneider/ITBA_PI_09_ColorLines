// utils.c
#include <stdlib.h>
#include "error.h"
#include "utils.h"

// creates a new matrix of height x width and 0es it
char ** newMatrix( const int height, const int width ){
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

void freeMatrix( char ** mat, const int height ){
	int i;
	for( i = 0 ; i < height ; i++ )
		free( mat[i] );
	free(mat);
}


bool entre( const int a, const int b, const int c ){
	return a<=b && b<c;
}