/**
* @file userInterface.c
* funtions for human-computer interaction
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "error.h"
#include "utils.h"
#include "defines.h"
#include "playGame.h"
#include "userInterface.h"


static char commandsBuffer[ MAX_PANEL_LINES ][ MAX_COM_LEN + 1 ];

static int commandsBufferPos = -1;


/**
* Clears the screen printing '\n'
*
*/

void clearScreen(){
	int i;
	backColor(BLACK);
	for( i = 0 ; i < MAX_TAB_DIM ; i++ )
		printf("\n");
	textAttr(CLEAR);
}

/**
* Draws the boards of each player in the game, the score and if necessary
* the time left, provided that it's not game over, in which case it prints
* just the score and "GAME OVER"
*
* @param game	contains all information about current game
*/

void drawTable( game_t * game ){
	int i, j, player, col;
	static const int colors[] = {
		BLACK, RED, LIGHT_BLUE, GREEN, YELLOW,
		VIOLET, PINK, SKY_BLUE, BROWN, LIGHT_GREEN
	};
	backColor(BLACK);
	textColor(WHITE);

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
				textColor( colors[ (int)col ] );
				if(col)
					printf("%d", (int)col );
				else
					printf(" ");
				textColor(WHITE);
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
	// draw points, time/player/GAME OVER
	char s[15], t[15];
	sprintf( s, "%%%ds", game->options.width * 4 - 10 );

	for( player = 0 ; player < game->numPlayers ; player++ ){

		i = ( game->state.next + player ) % game->numPlayers;

		printf("    SCORE %-4d", game->players[i].board.points );

		if( gameOver( game, player ) ){
			printf( s, "GAME OVER" );
		}else
		if( game->options.mode == TIMEMODE ){
			j = game->state.timeLeft - time(NULL) + game->state.lastTime;
			if( j >= 60 )
				sprintf( t, "%d:%d", j/60, j%60 );
			else
				sprintf( t, "%d", j );
			printf( s, t );
		}else
		if( game->options.mode == MULTIPLMODE ){
			sprintf( t, "Player %d", i + 1 );
			printf( s, t );
		}
		printf("   ");
	}

	textAttr(CLEAR);
}


/**
* Draws the text main panel. If @a str is provided, then the text is set to be
* it, else if @a str is NULL, the last text provided is used.
*
* @param str	text to print (can be NULL)
*/

void drawText( char * str ){
	static char buffer[ MAX_TEXT ] = "";
	if( str && str[0] )
		strcpy( buffer, str );
	backColor(BLACK);
	textColor(WHITE);
	printf("%s",buffer);
	textAttr(CLEAR);
}


/**
* Draws the text panel (secondary). If @a message is provided, then it is
* printed. All the previous commands and ' > ' are printed in either case.
*
* @param str	text to print (can be NULL or empty)
*/

void drawPanel( char * message ){
	char msg[ MAX_ERR_LEN ];
	int i;

	if( commandsBufferPos == -1 ){
		for( i = 0 ; i < MAX_PANEL_LINES ; i++ )
			sprintf( commandsBuffer[i], "\n" );
		commandsBufferPos = 0;
	}

	backColor(BLACK);

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
			textColor(WHITE);
		else
			textColor(GRAY);
		printf( "%s", commandsBuffer[ (commandsBufferPos-i+MAX_PANEL_LINES)
														% MAX_PANEL_LINES ] );
	}

	textColor(WHITE);
	printf( " > " );
	textAttr(CLEAR);
}


/**
* Reads a command from the standard input.
*
* @param str	buffer ( has at least MAX_COM_LEN size allocated )
*/

void askCommand( char * result ){
	backColor(BLACK);
	textColor(WHITE);

	result[ MAX_COM_LEN-5 ] = 0;

	raiseErrorIf( fgets( result, MAX_COM_LEN-3, stdin ), INPUTERROR, );

	if( result[ MAX_COM_LEN-5 ] ){		// we make sure it's \n terminated
		result[ MAX_COM_LEN-5 ] = '\n';
		clearBuffer();
	}
	sprintf( commandsBuffer[ commandsBufferPos++ ], " > %s", result );
	commandsBufferPos %= MAX_PANEL_LINES;

	textAttr(CLEAR);
}
