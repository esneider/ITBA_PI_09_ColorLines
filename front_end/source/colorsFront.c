// colorsFront.c
#include <stdlib.h>
#include "error.h"
#include "defines.h"
#include "game.h"
#include "commands.h"
#include "menu.h"
#include "userInterface.h"

int main(){

	game_t * game;
	char command[ MAX_COM_LEN ];
	char message[ MAX_ERR_LEN ];

	while(1){
		clearError();
		game = menu();
		
		if( game == NULL )
// 			printf("blablabla\n%s\n",errorMessage( errorCode() )); fflush(stdout);
			break;
		if( errorCode() != NOERROR ){
			drawText( errorMessage( errorCode() ) );
			continue;
		}
		message[0]=0;
		while(1){
			clearError();
			// start drawing
			clearScreen();
			drawTable( game, 0, NULL );
			drawPanel( message );
			// end drawing
			message[0]=0;
			askCommand( command );
			if( errorCode() != NOERROR ){
				drawText( errorMessage( errorCode() ) );
				continue;
			}
			if( !newCommand( game, command, message ) && !message[0] )
				break;
			if( errorCode() != NOERROR ){
				drawText( errorMessage( errorCode() ) );
				continue;
			}
		}
	}
	return EXIT_SUCCESS;
}