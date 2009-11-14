// colorsFront.c
#include <stdbool.h>
#include "error.h"
#include "defines.h"
#include "game.h"
#include "commands.h"
#include "menu.h"
#include "userInterface.h"

int main(){

	game_t * game;
	bool commSuccess;
	char command[ MAX_COM_LEN ];
	char error[ MAX_ERR_LEN ];

	while( true ){
		clearError();
		game = menu();
		if( game == NULL )
			break;
		if( errorCode() != NOERROR ){
			clearScreen();
			drawText( errorMessage( errorCode() ) );
			drawPanel( NULL );
			askCommand( command );
			continue;
		}
		commSuccess = true;
		while( true ){
			clearError();
			clearScreen();
			drawTable( game, 0, NULL );
			drawPanel( commSuccess ? NULL : error );
			askCommand( command );
			if( errorCode() != NOERROR ){
				clearScreen();
				drawText( errorMessage( errorCode() ) );
				drawPanel( NULL );
				askCommand( command );
				continue;
			}
			commSuccess = newCommand( game, command, error );
			if( !commSuccess && !error[0] )
				break;
		}
	}
}