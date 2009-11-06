#include <stdio.h>
#include "error.h"
#include "game.h"
#include "colors.h"
#include "commands.h"

int main(){
	clear_error();
	game * G = new_game(0,0,7,7,9,5,3);
	write_game(G,"el_juego.game");
	G = read_game("el_juego.game");
	printf("%d %d %d %d \n",G->height,G->width,G->p1points,G->colors);
}