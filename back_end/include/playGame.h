// playGame.h
#ifndef PLAYGAME_H
#define PLAYGAME_H

#include <stdbool.h>
#include "colors.h"
#include "game.h"

int winningPlay( game_t *game, int nPlayer, int x, int y, bool countPoints );

void randFill( game_t * game, int nPlayer, size_t cant, bool force );


#endif // PLAYGAME_H