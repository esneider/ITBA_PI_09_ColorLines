/**
* @file utils.h
* Contains useful functions that are used several times across the code.
*/

#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>


// creates a new matrix of 'height' x 'width' and 0es it
char ** newMatrix( int height, int width );

// frees a matrix created by newMatrix
void freeMatrix( char ** mat, int height );

// copies a matrix created by newMatrix
void copyMatrix( char ** from, char ** to, int height, int width );

// assert b \in [a,b)
bool entre( const int a, const int b, const int c );

// like 'entre' but with error mesage
bool validateInt( int a, int b, int c, char * err );

// clears stdin buffer
void clearBuffer();

// returns min(a,b)
int min( int a, int b );

// returns max(a,b)
int max( int a, int b );

// return the similarity of str1 and str2 based on their edit distance
// returns a number \in [0,1] 1=equal 0=different
double editDistance( const char * str1, const char * str2 );


#endif // UTILS_H