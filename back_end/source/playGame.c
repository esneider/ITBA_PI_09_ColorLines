// playGame.c
#include <stdbool.h>
#include "error.h"
#include "defines.h"
#include "utils.h"
#include "colors.h"
#include "playGame.h"

typedef struct {
	int dir[2];
}directions_t;

static int lookForLine(game_t *game, size_t x, size_t y, directions_t *directions);

bool notFree( game_t * game, int nPlayer, size_t x, size_t y, color c){
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

int 
winningPlay(game_t *game, size_t x, size_t y, bool player){
	int blanks=0;
	int previousblanks;
	int multilines = 0;
	int i;
	directions_t directions[]={ {{0,1}} , {{1,0}} , {{1,1}} , {{-1,1}} };
	for( i = 0 ; i < 4 ; i++){
		previousblanks = blanks;
		blanks += lookForLine(game, x, y, &directions[i]);
		if (previousblanks!= blanks)
			multilines++;
	}
	if (blanks>0){
		game->players[game->state.next].board.matrix[y][x] = 0;
		blanks++;
	}
	if (player && multilines){
		if (multilines > 1 || blanks-3 > game->options.tokensPerLine )
			game->players[game->state.next].board.points+= 8;
		else if (blanks == game->options.tokensPerLine)
			game->players[game->state.next].board.points+= 1;
		else if (blanks-1 == game->options.tokensPerLine)
			game->players[game->state.next].board.points+= 2;
		else if (blanks-2 == game->options.tokensPerLine)
			game->players[game->state.next].board.points+= 4;
		else if (blanks-3 == game->options.tokensPerLine)
			game->players[game->state.next].board.points+= 6;
	}
	game->players[game->state.next].board.emptySpots+= blanks;
	return blanks;
	
}

static int
lookForLine(game_t *game, size_t x, size_t y, directions_t *directions){
	int i, j, aux;
	// empiezo desde i = 1 porque i = 0 empezaria del x, y
	for(i = 1 ; game->players[game->state.next].board.matrix[y][x] == 
					game->players[game->state.next].board.matrix[y+i*directions->dir[0]][x+i*directions->dir[1]] &&
					entre(0, x+i*directions->dir[0], game->options.width) &&
					entre(0, y+i*directions->dir[1], game->options.height) ; i++)
		;
	aux = i-1;
	// mientras aux != 0 ir borrando
	while( aux ){
		game->players[game->state.next].board.matrix[y+aux*directions->dir[0]][x+aux*directions->dir[1]] = 0;
		aux--;
	}
	directions->dir[0] *= -1;
	directions->dir[1] *= -1;
	for(j = 1 ; game->players[game->state.next].board.matrix[y][x] == 
					game->players[game->state.next].board.matrix[y+i*directions->dir[0]][x+j*directions->dir[1]] && 
					entre(0, x+j*directions->dir[0], game->options.width) &&
					entre(0, y+j*directions->dir[1], game->options.height) ; j++)
		;
	aux = j-1;
	while( aux ){
		game->players[game->state.next].board.matrix[y+aux*directions->dir[0]][x+aux*directions->dir[1]] = 0;
		aux--;
	}
	return i+j-2;
}