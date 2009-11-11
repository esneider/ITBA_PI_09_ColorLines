#include <stdio.h>
#include "error.h"
#include "game.h"
#include "colors.h"
#include "commands.h"

int main(){
	clearError();
	options_t options;
	game_t * game = newGame(&options);
	writeGame(game,"elJuego.game");
	game = readGame("elJuego.game");
	printf("%d %d %d %d \n", game->options.height, game->options.width,
							game->options.numColors, game->options.mode );
}
