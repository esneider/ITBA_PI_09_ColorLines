// playGame.c
#include <stdbool.h>
#include <time.h>
#include "error.h"
#include "defines.h"
#include "utils.h"
#include "colors.h"
#include "playGame.h"
#include "userInterface.h"


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

typedef struct {
	int x,y;
}direction_t;

static int lookForLine( game_t * game, int x, int y, direction_t dir ){
	int i, dx, dy, tokens = 1;

	color c = game->players[game->state.next].board.matrix[y][x];
		// we count how many tokens of the same color are aligned
	dx = x + dir.x; dy = y + dir.y;
	while ( game->players[ game->state.next ].board.matrix[dy][dx] == c && 
			entre( 0, dx, game->options.width ) &&
			entre( 0, dy, game->options.height ) ){
				dx += dir.x;
				dy += dir.y;
				tokens++;
	}
	dx = x - dir.x; dy = y - dir.y;
	while ( game->players[ game->state.next ].board.matrix[dy][dx] == c && 
			entre( 0, dx, game->options.width ) &&
			entre( 0, dy, game->options.height ) ){
				dx -= dir.x;
				dy -= dir.y;
				tokens++;
	}
		// we erase the line
	if( tokens >= game->options.tokensPerLine ){
		for( i = 0 ; i < tokens ; i++ )
			dx += dir.x;
			dy += dir.y;
			game->players[ game->state.next ].board.matrix[dy][dx] = 0;
		}
		game->players[game->state.next].board.matrix[y][x] = c;
	return tokens;
/*	for(i = 1 ; game->players[game->state.next].board.matrix[y][x] == 
		game->players[game->state.next].board.matrix[y+i*directions->dir[0]][x+i*directions->dir[1]] &&
		entre(0, x+i*directions->dir[0], game->options.width) &&
		entre(0, y+i*directions->dir[1], game->options.height) ; i++)
		; 
	directions->dir[0] *= -1;
	directions->dir[1] *= -1;
	for(j = 1 ; game->players[game->state.next].board.matrix[y][x] == 
		game->players[game->state.next].board.matrix[y+i*directions->dir[0]][x+j*directions->dir[1]] && 
		entre(0, x+j*directions->dir[0], game->options.width) &&
		entre(0, y+j*directions->dir[1], game->options.height) ; j++)
		;*/
/*	if(i+j-2+1 >= game->options.tokensPerLine){
		aux = i-1;
		//mientras aux != 0 ir borrando
		while( aux ){
			game->players[game->state.next].board.matrix[y+aux*directions->dir[0]][x+aux*directions->dir[1]] = 0;
			aux--;
		}
		directions->dir[0] *= -1;
		directions->dir[1] *= -1;
		aux = i-1;
		while( aux ){
			game->players[game->state.next].board.matrix[y+aux*directions->dir[0]][x+aux*directions->dir[1]] = 0;
			aux--;
		}
	}
	return i+j-2;*/
}

int winningPlay( game_t *game, int x, int y, bool player ){

	int i, aux, emptySpots=0, lines = 0;
	direction_t directions[]={ {0,1}, {1,0}, {1,1}, {-1,1} };

	for( i = 0 ; i < 4 ; i++){
		aux = lookForLine( game, x, y, directions[i] );
		if(aux){
			lines++;
			emptySpots += aux;
		}
	}
	if(!emptySpots){
		emptySpots++;
		game->players[game->state.next].board.matrix[y][x] = 0;
		game->players[game->state.next].board.emptySpots += emptySpots;
		if( lines > 1 )
			game->players[game->state.next].board.points += 8;
		else
			switch( emptySpots - game->options.tokensPerLine ){
				case 0:
					game->players[game->state.next].board.points += 1;
					break;
				case 1:
					game->players[game->state.next].board.points += 2;
					break;
				case 2:
					game->players[game->state.next].board.points += 4;
					break;
				case 3:
					game->players[game->state.next].board.points += 6;
					break;
				default:
					game->players[game->state.next].board.points += 8;
					break;
			}
	}
	return emptySpots;
}
