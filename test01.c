#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "game.h"
#include "colors.h"
#include "commands.h"
#include "menu.h"
#include "ui.h"

int main(){
	clearError();
// 	options_t options ;
	game_t * game = menu();
	for( int i=0 ; i< game->options.width ; i++ )
		for( int j=0 ; j< game->options.height ; j++ )
			game->players[ game->state.next ].board.matrix[j][i] = rand()%game->options.numColors;
	writeGame(game,"test.gam");
	game = readGame("test.gam");
/*	printf("%d %d %d %d \n", game->options.height, game->options.width,
							game->options.numColors, game->options.mode );*/
	clearViewPort();
	drawTable( game );
}
