// playGame.c
#include <stdbool.h>
#include <time.h>
#include "error.h"
#include "defines.h"
#include "utils.h"
#include "colors.h"
#include "playGame.h"
#include "userInterface.h"


/**
* WTF?
*
*/

bool notFree( game_t * game, int nPlayer, size_t x, size_t y, color c){
	return false;
}
// TODO TODO TODO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// TODO TODO TODO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// TODO TODO TODO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!


/**
* Fills the board with a certain number of random tokens.
*
* @param game		contains all the information about the current game
* @param nPlayer	indicates the current player
* @param cant 		indicates the number of tokens about to be placed
* @param force		indicates if when a line is made, and tokens are erased,
*					tokens should still be filled until @a cant are reached
*/

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


/**
* Look for line, given a position [x,y] checks for tokens of the same 
* color. If tokens is greater than or equal to tokens per line,
* erases the line.
*
* @param game	Game structure.
* @param x 		Coordinate.
* @param y 		Coordinate.
* @param dir	Structure, where to move while checking for tokens.
* @return 		new emptySpots, tokens extrancted from the board.
*/

static int lookForLine( game_t * game, int x, int y, direction_t dir ){
	int i, dx, dy, tokens = 1;

	color c = game->players[game->state.next].board.matrix[y][x];
		// count how many tokens of the same color are aligned
	dx = x + dir.x; dy = y + dir.y;
	while ( entre( 0, dx, game->options.width ) &&
			entre( 0, dy, game->options.height ) &&
			game->players[ game->state.next ].board.matrix[dy][dx] == c ){
				dx += dir.x;
				dy += dir.y;
				tokens++;
	}
	dx = x - dir.x; dy = y - dir.y;
	while ( entre( 0, dx, game->options.width ) &&
			entre( 0, dy, game->options.height ) &&
			game->players[ game->state.next ].board.matrix[dy][dx] == c ){
				dx -= dir.x;
				dy -= dir.y;
				tokens++;
	}
		// erase the line
	if( tokens >= game->options.tokensPerLine ){
		for( i = 0 ; i < tokens ; i++ ){
			dx += dir.x;
			dy += dir.y;
			game->players[ game->state.next ].board.matrix[dy][dx] = 0;
		}
		game->players[game->state.next].board.matrix[y][x] = c;
		return tokens;
	}
	return 0;
}


/**
* Winning Play uses {@link lookForLine}, if tokens return by lookForLine is
* greater than 1 erases position [x,y], increases the score depending on
* number of lines made and tokens aligned.
*
* @param game	game structure
* @param x 		coordinate
* @param y 		coordinate
* @param 
*
* @return number of lines deleted
*
* @see lookForLine()
*/

int winningPlay( game_t *game, int x, int y, bool countPoints ){

	int i, aux, emptySpots=0, lines = 0;
	direction_t directions[]={ {0,1}, {1,0}, {1,1}, {-1,1} };

	for( i = 0 ; i < 4 ; i++){
		aux = lookForLine( game, x, y, directions[i] );
		if(aux){
			lines++;
			emptySpots += aux - 1;
		}
	}
	if(emptySpots){
		emptySpots++;
		game->players[game->state.next].board.matrix[y][x] = 0;
		game->players[game->state.next].board.emptySpots += emptySpots;
		if( countPoints ){
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
	}
	return emptySpots;
}
