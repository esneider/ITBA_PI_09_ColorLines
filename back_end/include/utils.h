/**
* @file utils.h
* Contains useful functions that are used several times across the code.
*/

#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdbool.h>


char ** newMatrix( size_t height, size_t width );

void freeMatrix( char ** mat, size_t height );

void copyMatrix( char ** to, char ** from, size_t height, size_t width );

bool entre( int a, int b, int c );

bool validateInt( int a, int b, int c, char * err );

void clearBuffer();

int min( int a, int b );

int max( int a, int b );

double editDistance( const char * str1, const char * str2 );


#endif // UTILS_H