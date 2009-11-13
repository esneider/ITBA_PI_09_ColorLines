// ui.h
#ifndef UI_H
#define UI_H

#include "game.h"

typedef struct{

int x,y,nBoard;

char character;

color_t color;

} modifier_t; 

void clearViewPort();

void drawTable( game_t * game );

#endif // UI_H