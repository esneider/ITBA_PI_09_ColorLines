/**
* @file colorsFront.c
* Contains the main function with the game loop.
*/

#include <stdlib.h>
#include <stdbool.h>
#include "error.h"
#include "defines.h"
#include "game.h"
#include "colorsBack.h"
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

int
main()
{
	game_t * game;
	char command[ MAX_COM_LEN ];
	char message[ MAX_ERR_LEN ];

	while( menu( &game ) ){
		if( errorCode() != NOERROR ){
			drawPanel( errorMessage( errorCode() ) );
			continue;
		}
		message[0]=0;
		while( !game->state.quit && !gameOver( game, game->state.next ) ){
			clearError();
			clearScreen();
			drawTable( game );
			drawPanel( message );
			askCommand( command );
			newCommand( game, command, message );
		}
		if( gameOver( game, game->state.next ) ){
			drawWinner( game );
			clearScreen();
			drawTable( game );
			drawPanel("GAME OVER\nPress ENTER to return to main menu\n");
			askCommand( command );
		}
		freeGame( game );
	}
	return EXIT_SUCCESS;
}
