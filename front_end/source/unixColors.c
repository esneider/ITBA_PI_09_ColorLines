/**
* unixColores.c
* Sets the colors if the operating system is linux.
*/

#include <stdio.h>

const char color2font_color[] = 
{ 30,31,32,33,34,35,36,37,90,91,92,93,94,95,96,97 };
const char color2font_bkcolor[] =
{ 40,41,42,43,44,45,46,47,100,101,102,103,104,105,106,107 };
const char attr2font_attr[] =
{ 0,10,1,4,5,7,22,24,25,27 };

/**
* Sets the fontcolor if the operating system is linux.
*
* @param	c	a code depending on the desired color
*/

void textcolor( color c ){
	if( !USE_COLORS ) return;
	printf( "\033[%dm", color2font_color[(int)c] );
}

/**
* Sets the backcolor if the operating system is linux.
*
* @param	c	a code depending on the desired color
*/

void backcolor( color c ){
	if( !USE_COLORS ) return;
	printf( "\033[%dm", color2font_bkcolor[(int)c] );
}

/**
* Sets the text attribute if the operating system is linux.
*
* @param	c	a code depending on the desired color
*/

void textattr( attr a ){
	if( !USE_COLORS ) return;
	printf( "\033[%dm", attr2font_attr[(int)a] );
}
