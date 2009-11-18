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
#include <stdio.h>
char dd[20];

// returns new emptySpots a.k.a. tokens extrancted from the board
static int lookForLine( game_t * game, int x, int y, direction_t dir ){
	int i, dx, dy, tokens = 1;

	color c = game->players[game->state.next].board.matrix[y][x];
	
// 	sprintf(dd,"%d %d\n",dir.x,dir.y);
//  	printf("%s",dd);
// 	drawPanel(dd);
// 	printf("bhjj\n");
// 	drawPanel("bhjj\n");
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

int winningPlay( game_t *game, int x, int y ){

	int i, aux, emptySpots=0, lines = 0;
	direction_t directions[]={ {0,1}, {1,0}, {1,1}, {-1,1} };

	for( i = 0 ; i < 4 ; i++){
		aux = lookForLine( game, x, y, directions[i] );
// 		sprintf(dd,"%d\n",aux);
// 		printf("%s",dd);
// 		drawPanel(dd);
		if(aux){
			lines++;
			emptySpots += aux - 1;
		}
	}
// 	sprintf(dd,"%d %d %d\n",emptySpots, lines, game->options.tokensPerLine);
// 	printf("%s",dd);
// 	drawPanel(dd);
	
	if(emptySpots){
		emptySpots++;
		game->players[game->state.next].board.matrix[y][x] = 0;
		game->players[game->state.next].board.emptySpots += emptySpots;
// 		sprintf(dd,"%d %d\n",emptySpots - game->options.tokensPerLine, lines);
// 		drawPanel(dd);
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
// 	sprintf(dd,"%d\n",game->players[game->state.next].board.points);
// 	drawPanel(dd);
	return emptySpots;
}
