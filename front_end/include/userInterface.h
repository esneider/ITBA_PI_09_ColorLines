// ui.h
#ifndef UI_H
#define UI_H

#include "game.h"
#include "colors.h"

void clearScreen();

void drawTable( game_t * game  );

void drawText( char * str );

void drawPanel( char * message );

void askCommand( char * result );


#endif // UI_H