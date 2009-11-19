/**
* @file colorsFront.c
* Contains the main function with the game loop.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "error.h"
#include "defines.h"
#include "game.h"
#include "commands.h"
#include "menu.h"
#include "userInterface.h"
#include "playGame.h"

/**
* This function is actually the hole game loop. It takes care of calling every
* other function, including the ones on the frontend, which print everything,
* and the ones on the back end, which process everything.
*
* @return a code to be handled by the operating system depending of the state
* 		  of the program
*
* @see menu()
* @see drawTable()
* @see drawText()
* @see drawPanel()
* @see askCommand()
* @see newCommand()
*/

int main(){

	game_t * game;
	char command[ MAX_COM_LEN ];
	char message[ MAX_ERR_LEN ];

	while( true ){
		clearError();
				// show menu
		game = menu();
				// error
		if( errorCode() != NOERROR ){
			drawPanel( errorMessage( errorCode() ) );
			continue;
		}
				// exit
		if( game == NULL )
			break;
				// actual game-loop
		message[0]=0;
		while( !gameOver( game, game->state.next ) ){
			clearError();
					// start drawing
			clearScreen();
			drawTable( game );
			drawPanel( message );
					// end drawing
					// ask command
			message[0]=0;
			askCommand( command );
					// check for error while asking command
			if( errorCode() != NOERROR ){
				drawText( errorMessage( errorCode() ) );
				continue;
			}
					// analize command
			if( !newCommand( game, command, message ) && !message[0] ){
				freeGame(game);
				break;
			}
					// check for error while analizing command
			if( errorCode() != NOERROR ){
				drawText( errorMessage( errorCode() ) );
				continue;
			}
		}
		if( gameOver( game, game->state.next ) ){
			clearScreen();
			drawTable( game );
			drawPanel("GAME OVER\nPress ENTER to return to main menu\n");
			askCommand( command );
		}
	}
	printf("\n");
	return EXIT_SUCCESS;
}