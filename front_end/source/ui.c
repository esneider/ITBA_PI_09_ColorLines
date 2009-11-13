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

void drawTable( game_t * game, int numModifiers, modifier_t modifiers[]  ){
	int i, j;
	static int colores[] = {GRIS,ROJO,AZUL_CLARO,VERDE,AMARILLO,CELESTE,ROSA,VIOLETA,MARRON,VERDE_CLARO};
	backcolor(NEGRO);
	textcolor(BLANCO);
	//Modifiquemos la tabla de acuerdo a los modifiers! MENTIRA, NO ANDA
	//for(i=0;i<numModifiers;i++){
	//	textcolor( modifiers[i]->color );
	//	game->players[ modifiers[i]-> nBoard ].board.matrix[ modifiers[i]-> x ][ modifiers[i]-> y ] = modifiers[i]->character;
	//}
	printf("\n");
	printf(" ");
	for( i=0 ; i < game->options.width ; i++ )
		printf("+---");
	//Si son 2 jugadores
	if (game->options.mode == 2){
		printf("+      ");
		for( i=0 ; i < game->options.width ; i++ )
		printf("+---");
	}
	printf("+\n");

	for( i=0 ; i < game->options.height ; i++ ){
		for( j=0 ; j< game->options.width; j++ ){

			printf(" | ");

 			textcolor( colores[ (int)game->players[ game->state.next ].board.matrix[i][j] ] );
			printf("%d", (int)game->players[ game->state.next ].board.matrix[i][j] );

			textcolor(BLANCO);
		}
		// si son 2 jugadores
		if (game->options.mode == 2){
			printf(" |      ");
			for( j=0 ; j< game->options.width; j++ ){

			printf(" | ");

 			textcolor( colores[ (int)game->players[ ((game->state.next)+1)% game->numplayers ].board.matrix[i][j] ] );

 			printf("%d", (int)game->players[ (game->state.next)+1)% game->numplayers ].board.matrix[i][j] );

			textcolor(BLANCO);
			}
		}
		printf(" |\n ");
		for(j=0; j < game->options.width; j++)
			printf("+---");
		//Si son 2 jugadores
		if (game->options.mode == 2){
			printf("+      ");
			for(j=0; j < game->options.width; j++)
				printf("+---");
		}
		printf("+\n");
	}
	textattr(CLEAR);
	printf("\n");
}
