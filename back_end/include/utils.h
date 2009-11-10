//utils.h
#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>


// creates a new matrix of height x width and 0es it
char ** newMatrix( const int height, const int width );

void freeMatrix( char ** mat, const int height );


// b \in [a,b)
bool entre( const int a, const int b, const int c );


#endif // UTILS_H