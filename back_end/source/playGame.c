/**
* @file playGame.c
* Functions for token handling
*/

#include <stdlib.h>
#include <time.h>
#include "error.h"
#include "defines.h"
#include "utils.h"
#include "colors.h"
#include "playGame.h"

typedef struct {
	int x,y;
}direction_t;


/**
* Looks for lines of the same color, intersecting (@a x,@a y). If lines are
* found, they are erased.
*
* @param game		game structure
* @param nPlayer	player number
* @param x 			coordinate
* @param y 			coordinate
* @param dir		line direction to check
*
* @return new empty spots, tokens extrancted from the board
*/

static int
lookForLine( game_t * game, int nPlayer, size_t x, size_t y, direction_t dir )
{
	int i, dx, dy, tokens = 1;

	color c = game->players[nPlayer].board.matrix[y][x];
		// count how many tokens of the same color are aligned
	dx = x + dir.x; dy = y + dir.y;
	while ( entre( 0, dx, game->options.width ) &&
			entre( 0, dy, game->options.height ) &&
			game->players[nPlayer].board.matrix[dy][dx] == c ){
				dx += dir.x;
				dy += dir.y;
				tokens++;
	}
	dx = x - dir.x; dy = y - dir.y;
	while ( entre( 0, dx, game->options.width ) &&
			entre( 0, dy, game->options.height ) &&
			game->players[nPlayer].board.matrix[dy][dx] == c ){
				dx -= dir.x;
				dy -= dir.y;
				tokens++;
	}
		// erase the line
	if( tokens >= game->options.tokensPerLine ){
		for( i = 0 ; i < tokens ; i++ ){
			dx += dir.x;
			dy += dir.y;
			game->players[nPlayer].board.matrix[dy][dx] = 0;
		}
		game->players[nPlayer].board.matrix[y][x] = c;
		return tokens;
	}
	return 0;
}


/**
* Checks for lines, erases them, actualizes emptySports and, if @a countPoints
* is true, then it also actualizes points.
*
* @param game			game structure
* @param nPlayer		player number
* @param x 				coordinate
* @param y 				coordinate
* @param countPoints	if false points won't be taken into account
*
* @return number of lines deleted
*
* @see lookForLine()
*/

int
winningPlay( game_t *game, int nPlayer, size_t x, size_t y, bool countPoints )
{
	int i, aux, emptySpots=0, lines = 0;
	direction_t directions[]={ {0,1}, {1,0}, {1,1}, {-1,1} };

	for( i = 0 ; i < 4 ; i++){
		aux = lookForLine( game, nPlayer, x, y, directions[i] );
		if(aux){
			lines++;
			emptySpots += aux - 1;
		}
	}
	if(emptySpots){
		emptySpots++;
		game->players[nPlayer].board.matrix[y][x] = 0;
		game->players[nPlayer].board.emptySpots += emptySpots;
		if( countPoints ){
			if( lines > 1 )
				game->players[nPlayer].board.points += 8;
			else
				switch( emptySpots - game->options.tokensPerLine ){
					case 0:
						game->players[nPlayer].board.points += 1;
						break;
					case 1:
						game->players[nPlayer].board.points += 2;
						break;
					case 2:
						game->players[nPlayer].board.points += 4;
						break;
					case 3:
						game->players[nPlayer].board.points += 6;
						break;
					default:
						game->players[nPlayer].board.points += 8;
						break;
				}
		}
	}
	return emptySpots;
}


/**
* Fills the board with a certain number of random tokens.
*
* @throws COMPUTATIONALERROR	if after @b MAX_WAITING_TIME time no solution
*								has been obtained (just on force mode)
*
* @param game		contains all the information about the current game
* @param nPlayer	player number
* @param cant 		indicates the number of tokens about to be placed
* @param force		indicates if when a line is made, and tokens are erased,
*					tokens should still be filled until @a cant are reached
*
* @see winningPlay()
*/

void
randFill( game_t * game, int nPlayer, size_t cant, bool force )
{
	int i, j, pos;
	time_t initTime = time(NULL);

	struct point{
		int x,y;
	} vec[ game->players[nPlayer].board.emptySpots ], aux;

	do{
		// fill vec with the coordinates of all the empty spots
		pos = 0;
		for( i = 0 ; i < game->options.height ; i++ )
			for( j = 0 ; j < game->options.width ; j++ )
				if( !game->players[nPlayer].board.matrix[i][j] )
					vec[pos++] = (struct point){j,i};
		// random shuffle vec
		for( i = 0 ; i < game->players[nPlayer].board.emptySpots ; i++ ){
			pos = rand() % game->players[nPlayer].board.emptySpots;
			aux = vec[i];
			vec[i] = vec[pos];
			vec[pos] = aux;
		}
		// fill with cant tokens
		pos = game->players[nPlayer].board.emptySpots;
		j = 0;
		for( i = 0 ; i < cant ; i++ ){
			if( game->players[nPlayer].board.emptySpots <= 0 || pos <= i ){
				break;
			}
			game->players[nPlayer].board.emptySpots--;
			game->players[nPlayer].board.matrix[ vec[i].y ][ vec[i].x ] =
										rand() % game->options.numColors + 1;

			j += 1 - winningPlay( game, nPlayer, vec[i].x, vec[i].y, false );
		}
		cant -= j;

		raiseErrorIf( time(NULL)-initTime < MAX_WAITING_TIME, COMPUTATIONALERROR,);
	}while( force && cant > 0 && game->players[nPlayer].board.emptySpots > 0 );
}


/**
* Checks if game is over for player @a nPlayer
*
* @param game		contains all the information about the current game
* @param nPlayer	player number
*
* @returns true if game is over for player @a nPlayer. false otherwise
*/

bool
gameOver( game_t * game, int nPlayer )
{
	return ( game->options.mode == TIMEMODE
			&& game->state.timeLeft - time(NULL) + game->state.lastTime <= 0 )
			|| game->players[nPlayer].board.emptySpots <= 0;
}
