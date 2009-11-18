// utils.h
// potpourri of useful functions/procedures
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


#endif // UTILS_H