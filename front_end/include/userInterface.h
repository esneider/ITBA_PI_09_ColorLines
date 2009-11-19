/**
* @file userInterface.h
* Funtions for human-computer interaction
*/

#ifndef UI_H
#define UI_H

#include "game.h"
#include "colors.h"

void clearScreen();

void drawTable( game_t * game  );

void drawText( const char * message );

void drawPanel( const char * message );

void askCommand( char * result );


#endif // UI_H