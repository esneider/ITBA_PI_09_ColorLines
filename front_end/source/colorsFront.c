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
	bool quit;
	bool exit = false;
	char command[ MAX_COM_LEN ];
	char error[ MAX_ERR_LEN ];

	while( !exit ){
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
		quit = false;
		error[0] = 0;
		while( !quit ){
			clearError();
			clearScreen();
			drawTable( game, 0, NULL );
			drawPanel( error );
			askCommand( command );
			if( errorCode() != NOERROR ){
				clearScreen();
				drawText( errorMessage( errorCode() ) );
				drawPanel( NULL );
				askCommand( command );
				continue;
			}
			if( !newCommand( game, command, error ) && !error[0] ){
				quit = true;
			}
		}
	}
}