// ui.h
#ifndef UI_H
#define UI_H

#include "game.h"
#include "colors.h"

typedef struct{
	int x,y,nBoard;
	char character;
	color color;
} modifier_t;

void clearScreen();

void drawTable( game_t * game, int numModifiers, modifier_t * modifiers  );

void drawText( char * str );

void drawPanel( char * message );

void askCommand( char * result );


#endif // UI_H