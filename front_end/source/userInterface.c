// ui.c
#include <stdio.h>
#include <string.h>
#include "error.h"
#include "defines.h"
#include "userInterface.h"


static char commandsBuffer[ MAX_PANEL_LINES ][ MAX_COM_LEN + 1 ];

static int commandsBufferPos = -1;



void clearScreen(){
	int i;
	backcolor(NEGRO);
	for( i = 0 ; i < MAX_TAB_DIM ; i++ )
		printf("\n");
	textattr(CLEAR);
}


void drawTable( game_t * game, int numModifiers, modifier_t * modifiers  ){
	int i, j, player, col;
	static const int colores[] = {
		NEGRO, ROJO, AZUL_CLARO, VERDE, AMARILLO,
		VIOLETA, ROSA, CELESTE, MARRON, VERDE_CLARO
	};
	backcolor(NEGRO);
	textcolor(BLANCO);

	printf("\n   ");
	for( player = 0 ; player < game->numPlayers ; player++ ){
		for( i = 0 ; i < game->options.width ; i++ )
			printf("  %-2d",i);
		printf("       ");
	}

	if( HOR_LINES ){
		printf("\n   ");
		for( player = 0 ; player < game->numPlayers ; player++ ){
			for( i = 0 ; i < game->options.width ; i++ )
				printf("+---");
			printf("+      ");
		}
	}
	printf("\n");

	for( i = 0 ; i < game->options.height ; i++ ){

		for( player = 0 ; player < game->numPlayers ; player++ ){

			printf("%2d",i);
			for( j = 0 ; j < game->options.width; j++ ){

				col = game->players[ 
						( game->state.next + player ) % game->numPlayers
				].board.matrix[i][j];

				printf(" | ");
				textcolor( colores[ (int)col ] );
				printf("%d", (int)col );
				textcolor(BLANCO);
			}
			printf(" |   ");
		}
		if( HOR_LINES ){
			printf("\n   ");
			for( player = 0 ; player < game->numPlayers ; player++ ){
				for(j=0; j < game->options.width; j++)
					printf("+---");
				printf("+      ");
			}
		}
		printf("\n");
	}

	char s[10], t[12];
	for( player = 0 ; player < game->numPlayers ; player++ ){
		printf("    %4d", game->players[ player ].board.points );
// 		if( game->options.mode == TIMEMODE ){
// 			sprintf( s, "%%-%dd", game->options.width-4 );
// 			printf( s, game->state.timeLeft );
// 		}else
		if( game->options.mode == MULTIPLMODE ){
			sprintf( t, "Player %d",
						( game->state.next + player ) % game->numPlayers + 1 );
			sprintf( s, "%%%ds", game->options.width * 4 - 4  );
			printf( s, t );
		}
		printf("   ");
	}

	textattr(CLEAR);
}

void drawText( char * str ){
	static char buffer[ MAX_TEXT ] = "";
	if( str && str[0] )
		strcpy( buffer, str );
	backcolor(NEGRO);
	textcolor(BLANCO);
	printf("%s",buffer);
	textattr(CLEAR);
}


void drawPanel( char * message ){
	char msg[ MAX_ERR_LEN ];
	int i;

	if( commandsBufferPos == -1 ){
		for( i = 0 ; i < MAX_PANEL_LINES ; i++ )
			sprintf( commandsBuffer[i], "\n" );
		commandsBufferPos = 0;
	}

	backcolor(NEGRO);

	if( message && message[0] ){
		sprintf( msg, "%s", message );
		char * aux = strtok( msg, "\n" );
		while( aux ){
			sprintf( commandsBuffer[ commandsBufferPos++ ], "%s\n", aux );
			commandsBufferPos %= MAX_PANEL_LINES;
			aux = strtok( NULL, "\n" );
		}
	}

	printf("\n\n");
	for( i = PANEL_LINES ; i > 0 ; i-- ){
		if( commandsBuffer[ (commandsBufferPos-i+MAX_PANEL_LINES) 
												% MAX_PANEL_LINES ][1] == '>' )
			textcolor(BLANCO);
		else
			textcolor(GRIS);
		printf( "%s", commandsBuffer[ (commandsBufferPos-i+MAX_PANEL_LINES)
														% MAX_PANEL_LINES ] );
	}

	textcolor(BLANCO);
	printf( " > " );
	textattr(CLEAR);
}

void askCommand( char * result ){
	backcolor(NEGRO);
	textcolor(BLANCO);

	raiseErrorIf( fgets( result, MAX_COM_LEN, stdin ), INPUTERROR, );

	if( result[ MAX_COM_LEN-2 ] )		// we make sure it's \n terminated
		result[ MAX_COM_LEN-2 ] = '\n';

	sprintf( commandsBuffer[ commandsBufferPos++ ], " > %s", result );
	commandsBufferPos %= MAX_PANEL_LINES;

	textattr(CLEAR);
}
