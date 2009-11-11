#include <stdio.h>
#include "error.h"
#include "game.h"
#include "colors.h"
#include "commands.h"
#include "menu.h"

int main(){
	clearError();
// 	options_t options ;
	game_t * game = menu();
	writeGame(game,"test.gam");
	game = readGame("test.gam");
	printf("%d %d %d %d \n", game->options.height, game->options.width,
							game->options.numColors, game->options.mode );
}
