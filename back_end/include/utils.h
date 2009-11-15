// utils.h
// potpourri of useful functions/procedures
#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>


// creates a new matrix of 'height' x 'width' and 0es it
char ** newMatrix( const int height, const int width );

// frees a matrix of created by newMatrix
void freeMatrix( char ** mat, const int height );

// assert b \in [a,b)
bool entre( const int a, const int b, const int c );

// like 'entre' but with error mesage
bool validateInt( int a, int b, int c, char * err );

// clears stdin buffer
void clearBuffer();

int min( int a, int b );


#endif // UTILS_H