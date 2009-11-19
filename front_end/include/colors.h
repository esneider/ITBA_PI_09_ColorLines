/**
* @file colors.h
* Contains console color functions depending on the OS.
*/

#ifndef COLORS_H
#define COLORS_H


typedef enum {
	BLACK         ,
	RED           ,
	GREEN         ,
	BROWN         ,
	BLUE          ,
	VIOLET        ,
	SKY_BLUE      ,
	LIGHT_GREY    ,
	GRAY          ,
	PINK          ,
	LIGHT_GREEN   ,
	YELLOW        ,
	LIGHT_BLUE    ,
	LIGHT_VIOLET  ,
	LIGHT_SKY_BLUE,
	WHITE

} color;

typedef enum {
	CLEAR        ,
	NONE         ,

	BOLD         ,
	UNDERLINE    ,
	BLINK        ,
	INVERTED     ,

	NO_BOLD      ,
	NO_UNDERLINE ,
	NO_BLINK     ,
	NO_INVERTED

} attr;

void textColor( color c );

void backColor( color c );

void textAttr( attr a );


#endif