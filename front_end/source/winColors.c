// winColors.c
#include <windows.h>
#include <wincon.h>
#include <stdio.h>

const char crazyWinColorsMap[] = {
	/*NEGRO*/NEGRO,/*ROJO*/AZUL,/*VERDE*/VERDE,/*MARRON*/CELESTE,
	/*AZUL*/ROJO,/*VIOLETA*/VIOLETA,/*CELESTE*/MARRON,/*GRIS_CLARO*/GRIS_CLARO,
	/*GRIS*/GRIS,/*ROSA*/AZUL_CLARO,/*VERDE_CLARO*/VERDE_CLARO,/*AMARILLO*/CELESTE_CLARO,
	/*AZUL_CLARO*/ROSA,/*VIOLETA_CLARO*/VIOLETA_CLARO,/*CELESTE_CLARO*/AMARILLO,
	/*BLANCO*/BLANCO
};

const char color2font_color[] = 
{ 0, FOREGROUND_RED | FOREGROUND_INTENSITY, FOREGROUND_GREEN, FOREGROUND_RED, FOREGROUND_BLUE,
  FOREGROUND_BLUE | FOREGROUND_RED, FOREGROUND_GREEN | FOREGROUND_BLUE, 
  FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, FOREGROUND_RED | FOREGROUND_GREEN,
  FOREGROUND_RED | FOREGROUND_INTENSITY, FOREGROUND_GREEN | FOREGROUND_INTENSITY,
  FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, FOREGROUND_BLUE | FOREGROUND_INTENSITY,
  FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY,
  FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
  FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
};

const char color2font_bkcolor[] = 
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
* Sets the fontcolor if the operating system is windows.
*
* @param	c	a code depending on the desired color
*/
void textcolor( color c ){
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
* Sets the backcolor if the operating system is windows.
*
* @param	c	a code depending on the desired color
*/
void backcolor( color c ){
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
* Sets the text attribute if the operating system is windows.
*
* @param	c	a code depending on the desired color
*/
void textattr( attr a ){
	if( !USE_COLORS ) return;
	if( a == CLEAR ){
		backcolor(NEGRO);
		frontcolor(BLANCO);
	}
}
