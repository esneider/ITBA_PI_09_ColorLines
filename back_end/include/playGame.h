// playGame.h
#ifndef PLAYGAME_H
#define PLAYGAME_H

#include <stdbool.h>
#include "colors.h"
#include "game.h"

bool notFree( game_t * game, int nPlayer, size_t x, size_t y, color c);

void randFill( game_t * game, int nPlayer, size_t cant, bool force );

int winningPlay( game_t *game, int x, int y, bool countPoints );


#endif // PLAYGAME_H