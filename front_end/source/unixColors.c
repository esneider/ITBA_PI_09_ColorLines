/**
* @file unixColors.c
* Console colors in Unix.
*/

#include <stdio.h>

static const char color2font_color[] =
{ 30,31,32,33,34,35,36,37,90,91,92,93,94,95,96,97 };
static const char color2font_bkcolor[] =
{ 40,41,42,43,44,45,46,47,100,101,102,103,104,105,106,107 };
static const char attr2font_attr[] =
{ 0,10,1,4,5,7,22,24,25,27 };


/**
* Sets the font color.
*
* @param c	color
*/

void
textColor( color c )
{
	if( !USE_COLORS ) return;
	printf( "\033[%dm", color2font_color[(int)c] );
}


/**
* Sets the background color.
*
* @param c	color
*/

void
backColor( color c )
{
	if( !USE_COLORS ) return;
	printf( "\033[%dm", color2font_bkcolor[(int)c] );
}


/**
* Sets text attributes.
*
* @param a attribute
*/

void
textAttr( attr a )
{
	if( !USE_COLORS ) return;
	printf( "\033[%dm", attr2font_attr[(int)a] );
}
