/**
* @file utils.c
* Contains useful functions that are used several times across the code.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "error.h"
#include "defines.h"
#include "utils.h"


/**
* Creates a new matrix and starts its elemnts in 0.
*
* @throws MEMORYERROR if there was a problem while allocating memory
*
* @param height	height of the new matrix
* @param width	width of the new matrix
*
* @return a matrix of characters. every element in the matrix is a 0
*
* @see freeMatrix()
*/

char ** newMatrix( size_t height, size_t width ){
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
* Frees the reserved memory for a matrix created by {@link newMatrix()}.
*
* @param mat	the matrix of characters about to be freed
* @param height	the height of the matrix
*
* @see newMatrix()
*/

void freeMatrix( char ** mat, size_t height ){
	int i;
	for( i = 0 ; i < height ; i++ )
		free( mat[i] );
	free(mat);
}


/**
* Copies a matrix to another one.
*
* @param[out] to	output matrix in wich from was copied
* @param from		the matrix to be copied
* @param height		the height of the matrix
* @param width 		the width of the matrix
*
* @see newMatrix()
* @see freeMatrix()
*/

void copyMatrix( char ** to, char ** from, size_t height, size_t width ){
	int i;
	for( i = 0 ; i < height ; i++ )
		memcpy( to[i], from[i], width );
}


/**
* Checks if @a b is in the interval [@a a,@a c).
*
* @param a	lower limit
* @param b	number to be compared
* @param c 	the upper limit
*
* @return	true if @a b is in [@a a,@a c), otherwise, false
*/

bool entre( int a, int b, int c ){
	return a<=b && b<c;
}


/**
* Checks if a number is between to other numbers. Might return an error.
*
* @param a			lower limit
* @param b			number to be compared
* @param c 			the upper limit
* @param[out] err	buffer to print the @b RANGEERROR message
*
* @return	true if @a b is in [@a a,@a c), otherwise, false
*
* @see entre()
*/

bool validateInt( int a, int b, int c, char * err ){
	if( !entre(a,b,c) ){
		sprintf( err, "Range error:\nIt must belong to the "
		"interval [%d,%d]", a, c-1 );
		return false;
	}
	return true;
}


/**
* Clears the standard input buffer.
*
*/

void clearBuffer(){
	while(getchar() != '\n');
}


/**
* Calculates the minimum between two numbers.
*
* @param a	first number
* @param b	second number
*
* @return the minimum beetween @a a and @a b
*/

int min( int a, int b ){
	return (a<=b)?a:b;
}


/**
* Calculates the maximum between two numbers.
*
* @param a	first number
* @param b	second number
*
* @return the maximum beetween @a a and @a b
*/

int max( int a, int b ){
	return (a>=b)?a:b;
}


/**
* Calculates the edit distance between @a str1 and @a str2
* (a.k.a Damerau–Levenshtein distance).
*
* @param str1	first string
* @param str2	second string
*
* @return a number between 0 and 1 that indicates similarity between
*		  @a str1 and @a str2. greater is better
*/

double editDistance( const char * str1, const char * str2 ){
	int i,j,cost=0;
	int s1len=strlen(str1);
	int s2len=strlen(str2);
	int mat[ s1len+1 ][ s2len+1 ];

	if( min( s1len, s2len ) < MIN_EDIT_LEN || max( s1len, s2len ) > MAX_EDIT_LEN )
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
