// utils.h
// potpourri of useful functions/procedures
#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>


#define MAX_ERR_LEN 200
#define MAX_ARGS 10
#define MAX_COM_LEN 100

#define MIN_MINUTES 1
#define MAX_MINUTES 10000 // Max playing time: 1 week: 7 * 24 * 60 = 10080
#define MIN_TAB_DIM 5
#define MAX_TAB_DIM 500 // I don't think you have a bigger monitor
#define MIN_COLORS 2
#define MAX_COLORS 9
#define MIN_TOK_PER_LINE 3
#define MAX_TOK_PER_LINE 700 // 500 * sqrt(2) = 707


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

#endif // UTILS_H