// utils.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "error.h"
#include "utils.h"

/**
* Creates a new matrix and starts its elemnts in 0.
*
* @param	height	height of the new matrix
* @param 	width	width of the new matrix
* @returns			a matrix of characters. every element in the matrix is a 0
*/
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
/**
* Frees the reserved memory for a matrix.
*
* @param	mat		the matrix of characters about to be freed
* @param	height	the height of the matrix
* @return 	void	
*/
void freeMatrix( char ** mat, int height ){
	int i;
	for( i = 0 ; i < height ; i++ )
		free( mat[i] );
	free(mat);
}
/**
*	Copies a matrix to another one.
*
* @param	from	the matrix to be copied
* @param	to		output matrix in wich from was copied
* @param 	height	the height of the matrix
* @param	width 	the width of the matrix
* @return	void
*/
void copyMatrix( char ** from, char ** to, int height, int width ){
	for( height-- ; height >= 0 ; height-- )
		memcpy( to[height], from[height], width );
}
/**
* Checks if a number is between two other numbers.
*
* @param	a	the lower limit b is permitted to be
* @param	b	the number about to be compared
* @param	c 	the upper limit b is permitted to be
* @return		true if b is between a and c, otherwise, false
*/
bool entre( const int a, const int b, const int c ){
	return a<=b && b<c;
}
/**
* Clears the entrance buffer.
*
* @return	void
*/
void clearBuffer(){
	while(getchar() != '\n');
}
/**
* Checks if a number is between to other numbers. Might return an error.
*
* @param	a	the lower limit b is permitted to be
* @param	b	the number about to be compared
* @param	c 	the upper limit b is permitted to be
* @param	err	an output string containing the type of error in case there is one
* @return		true if b is between a and c, otherwise, false
*/
bool validateInt( int a, int b, int c, char * err ){
	if( !entre(a,b,c) ){
		sprintf( err, "Rank error:\nIt must belong to the "
		"interval [%d,%d]", a, c-1 );
		return false;
	}
	return true;
}
/**
* Calculetes the minimum between two numbers.
*
* @param a	one of the numbers to be compared
* @param b	the other number to be compared
* @return	the minimum of the two numbers
*/
int min( int a, int b ){
	return (a<=b)?a:b;
}
/**
* Calculetes the maximum between two numbers.
*
* @param a	one of the numbers to be compared
* @param b	the other number to be compared
* @return	the maximum of the two numbers
*/
int max( int a, int b ){
	return (a>=b)?a:b;
}
/**
* WTF HACE??????????????
*/
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
