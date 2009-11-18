// utils.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

int max( int a, int b ){
	return (a>=b)?a:b;
}

double editDistance( const char * str1, const char * str2 ){
	int i,j,cost=0;
	int s1len=strlen(str1);
	int s2len=strlen(str2);
	int mat[ s1len+1 ][ s2len+1 ];
	
	if( min( s1len, s2len ) < 3 )
		return 0;
	
	for( i = 0 ; i <= s1len ; i++ )
		mat[i][0] = i;
	for( i = 0 ; i <= s2len ; i++ )
		mat[0][i] = i;
	
	for( i = 0 ; i <= s1len ; i++ )
		for( j = 0 ; j <= s2len ; j++ ){
			cost = toupper(str1[i]) != toupper(str2[j]);
			mat[i+1][j+1] =
			min( mat[i][j+1]+1, min( mat[i+1][j]+1, mat[i][j]+cost ) );
			if( i && j && toupper(str1[i]) == toupper(str2[j-1])
						&& toupper(str1[i-1]) == toupper(str2[j]) )
				mat[i+1][j+1] = min( mat[i+1][j+1], mat[i-1][j-1] + cost );
		}
		return 1 - mat[s1len][s2len] / (double)max( s1len, s2len );
}
