// ui.c
#include <stdio.h>
#include "utils.h"
#include "ui.h"

void clearViewPort(){
	int i;
	backcolor(NEGRO);
	for( i = 0 ; i < MAX_TAB_DIM ; i++ )
		printf("\n");
	textattr(CLEAR);
}

void drawTable( game_t * game, int numModifiers, modifier_t modifiers[]  ){
	int i, j, player, col;
	static int colores[] = {NEGRO,ROJO,AZUL_CLARO,VERDE,AMARILLO,CELESTE,ROSA,VIOLETA,MARRON,VERDE_CLARO};
	backcolor(NEGRO);
	textcolor(BLANCO);

	printf("\n ");
	
	for( player = 0 ; player < game->numPlayers ; player++ ){
		for( i=0 ; i < game->options.width ; i++ )
			printf("+---");
		printf("+      ");
	}
	printf("\n");

	for( i=0 ; i < game->options.height ; i++ ){

		for( player = 0 ; player < game->numPlayers ; player++ ){

			for( j=0 ; j< game->options.width; j++ ){

				col = game->players[ 
							( game->state.next + player ) % game->numPlayers
						].board.matrix[i][j];

				printf(" | ");
				textcolor( colores[ (int)col ] );
				printf("%d", (int)col );
				textcolor(BLANCO);
			}
			printf(" |     ");
		}
		printf("\n ");
		
		for( player = 0 ; player < game->numPlayers ; player++ ){
			for(j=0; j < game->options.width; j++)
				printf("+---");
			printf("+      ");
		}
		printf("\n");
	}
	textattr(CLEAR);
}

/*void drawText( char * str ){
	printf("");*/
}