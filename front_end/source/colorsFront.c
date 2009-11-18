// colorsFront.c
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
		while( true ){
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
			if( !newCommand( game, command, message ) && !message[0] )
				break;
					// check for error while analizing command
			if( errorCode() != NOERROR ){
				drawText( errorMessage( errorCode() ) );
				continue;
			}
		}
	}
	printf("\n");
	return EXIT_SUCCESS;
}