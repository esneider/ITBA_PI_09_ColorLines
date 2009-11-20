/**
* @file winColors.c
* Console colors in Windows.
*/

#include <windows.h>
#include <wincon.h>
#include <stdio.h>

static const char crazyWinColorsMap[] = {
	/*NEGRO*/BLACK,/*ROJO*/BLUE,/*VERDE*/GREEN,/*MARRON*/SKY_BLUE,
	/*AZUL*/RED,/*VIOLETA*/VIOLET,/*CELESTE*/BROWN,/*GRIS_CLARO*/LIGHT_BLUE,
	/*GRIS*/GRAY,/*ROSA*/LIGHT_BLUE,/*VERDE_CLARO*/LIGHT_GREEN,/*AMARILLO*/LIGHT_BLUE_SKY,
	/*AZUL_CLARO*/PINK,/*VIOLETA_CLARO*/LIGHT_VIOLET,/*CELESTE_CLARO*/YELLOW,
	/*BLANCO*/WHITE
};

static const char color2font_color[] =
{ 0, FOREGROUND_RED | FOREGROUND_INTENSITY, FOREGROUND_GREEN, FOREGROUND_RED, FOREGROUND_BLUE,
  FOREGROUND_BLUE | FOREGROUND_RED, FOREGROUND_GREEN | FOREGROUND_BLUE,
  FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, FOREGROUND_RED | FOREGROUND_GREEN,
  FOREGROUND_RED | FOREGROUND_INTENSITY, FOREGROUND_GREEN | FOREGROUND_INTENSITY,
  FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, FOREGROUND_BLUE | FOREGROUND_INTENSITY,
  FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY,
  FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
  FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
};

static const char color2font_bkcolor[] =
{ 0, BACKGROUND_RED | BACKGROUND_INTENSITY, BACKGROUND_GREEN, BACKGROUND_RED, BACKGROUND_BLUE,
  BACKGROUND_BLUE | BACKGROUND_RED, BACKGROUND_GREEN | BACKGROUND_BLUE,
  BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE, BACKGROUND_RED | BACKGROUND_GREEN,
  BACKGROUND_RED | BACKGROUND_INTENSITY, BACKGROUND_GREEN | BACKGROUND_INTENSITY,
  BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY, BACKGROUND_BLUE | BACKGROUND_INTENSITY,
  BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY,
  BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY,
  BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY
};


/**
* Sets the font color.
*
* @param c	color
*/

void
textColor( color c )
{
	WORD wColor;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if( !USE_COLORS ) return;
	if(GetConsoleScreenBufferInfo(hStdOut, &csbi)){
		wColor = (csbi.wAttributes & 0xF0) + ( crazyWinColorsMap[c] & 0x0F);
		SetConsoleTextAttribute(hStdOut, wColor);
	}
}


/**
* Sets the background color.
*
* @param c	color
*/

void
backColor( color c )
{
	WORD wColor;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if( !USE_COLORS ) return;
	if(GetConsoleScreenBufferInfo(hStdOut, &csbi)){
		wColor = (csbi.wAttributes & 0x0F) + ( crazyWinColorsMap[c] & 0xF0);
		SetConsoleTextAttribute(hStdOut, wColor);
	}
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
	if( a == CLEAR ){
		backcolor(BLACK);
		frontcolor(WHITE);
	}
}
