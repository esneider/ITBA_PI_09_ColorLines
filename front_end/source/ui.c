// ui.c
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "ui.h"

void clearScreen(){
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

void drawText( char * str ){
	static char buffer[ MAX_TEXT ] = "";
	if( str )
		strcpy( buffer, str );
	else{
		backcolor(NEGRO);
		textcolor(BLANCO);
		printf("%s",buffer);
		textattr(CLEAR);
	}
}

void askCommand( char * message, char * result ){
	static int pos = -1;
	static char buffer[ MAX_PANEL_LINES ][ MAX_COM_LEN + 1 ];
	int i;
	
	if( pos == -1 ){
		int i;
		for( i = 0 ; i < MAX_PANEL_LINES ; i++ )
			sprintf( buffer[i], "\n" );
		pos++;
	}

	backcolor(NEGRO);
	
	if( message && message[0] ){
		char * aux = strtok( message, "\n" );
		while( aux ){
			sprintf( buffer[pos++], "%s\n", aux );
			pos %= MAX_PANEL_LINES;
			aux = strtok( NULL, "\n" );
		}
	}

	printf("\n\n");
	for( i = 0 ; i < MAX_PANEL_LINES ; i++ ){
		if( buffer[ (pos+i) % MAX_PANEL_LINES ][1] == '>' )
			textcolor(BLANCO);
		else
			textcolor(GRIS);
		printf( "%s", buffer[ (pos+i) % MAX_PANEL_LINES ] );
	}

	textcolor(BLANCO);
	printf( " > " );
										// remember to catch fgets error (NULL)
	result = fgets( result, MAX_COM_LEN, stdin );
										// we make sure it's \n terminated
	if( result[ MAX_COM_LEN-2 ] )
		result[ MAX_COM_LEN-2 ] = '\n';

	sprintf( buffer[pos++], " > %s", result );
	pos %= MAX_PANEL_LINES;

	textattr(CLEAR);
}
