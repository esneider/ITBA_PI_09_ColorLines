// ui.c
#include <stdio.h>
#include "utils.h"
#include "colors.h"
#include "ui.h"

void clearViewPort(){
	int i;
	for( i = 0 ; i < MAX_TAB_DIM ; i++ )
		printf("\n");
}

void drawTable( game_t * game ){
	int i, j;
// 	static int colores[] = {GRIS,ROJO,AZUL_CLARO,VERDE,AMARILLO,CELESTE,ROSA,VIOLETA,MARRON,VERDE_CLARO};
	backcolor(NEGRO);
	textcolor(BLANCO);
	printf("\n");
	printf(" ");
	for( i=0 ; i < game->options.width ; i++ )
		printf("+---");
	printf("+\n");

	for( i=0 ; i < game->options.height ; i++ ){
		for( j=0 ; j< game->options.width; j++ ){

			printf(" | ");

// 			textcolor( colores[ (int)game->players[ game->state.next ].board.board[i][j] ] );

// 			printf("%d", (int)game->players[ game->state.next ].board.board[i][j] );

			textcolor(BLANCO);
		}
		printf(" |\n ");
		for(j=0; j < game->options.width; j++)
			printf("+---");
		printf("+\n");
	}
	textattr(CLEAR);
	printf("\n");
}