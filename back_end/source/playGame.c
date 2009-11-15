// playGame.c
#include <stdbool.h>
#include "error.h"
#include "defines.h"
#include "utils.h"
#include "colors.h"
#include "playGame.h"

static bool notFree( game_t * game, int nPlayer, size_t x, size_t y, color c){
	return false;
}

void randFill( game_t * game, int nPlayer, size_t cant, bool force ){
	int i;
	size_t x, y;
	color c;

	for( i = 0 ; i < cant ; i++ ){
		if( game->players[nPlayer].board.emptySpots <= 0 )
			break;
		do{
			x = rand() % game->options.width;
			y = rand() % game->options.height;
			c = rand() % game->options.numColors + 1;
		}while( game->players[nPlayer].board.matrix[y][x] != 0 ||
				( force && notFree( game, nPlayer, x, y, c ) ) );
		game->players[nPlayer].board.matrix[y][x] = c;
		game->players[nPlayer].board.emptySpots--;
	}
}
