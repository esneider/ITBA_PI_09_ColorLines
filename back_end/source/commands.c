// commands.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "error.h"
#include "utils.h"
#include "defines.h"
#include "playGame.h"
#include "commands.h"

bool movePiece ( game_t * game, int argc, char ** argv, char * err );
bool save      ( game_t * game, int argc, char ** argv, char * err );
bool buyItem   ( game_t * game, int argc, char ** argv, char * err );
bool throwItem ( game_t * game, int argc, char ** argv, char * err );
bool undo      ( game_t * game, int argc, char ** argv, char * err );
bool quit      ( game_t * game, int argc, char ** argv, char * err );
bool help      ( game_t * game, int argc, char ** argv, char * err );
bool roflcopter( game_t * game, int argc, char ** argv, char * err );

typedef struct{
	char com[ MAX_COM_LEN ];
	bool (* func )( game_t *, int, char **, char * );
} command_t;

const command_t commands[] = {
	{"[", movePiece},
	{"save", save},
	{"buy", buyItem},
	{"throw", throwItem},
	{"undo", undo},
	{"quit", quit},
	{"help", help},
	{"ROFLcopter",roflcopter}
};

/**
* Handles the given command. 
* 
* @param game 	contains every information needed of the current game
* @param s		containins the command about to be processed
* @param err    its an output containing the type of error
*
* @return 		false if there is an error, otherwise true
*/
bool newCommand( game_t * game, char * s, char * err ){
	int i;
	bool sol;
	int argc;
	char ** argv;

	clearError();

	argv = newMatrix( MAX_ARGS, MAX_COM_LEN );

	if( errorCode() != NOERROR ){
		sprintf( err, "%s", errorMessage( errorCode() ) );
		return false;
	}
	//reading the given command
	argc = sscanf( s, "%s %s %s %s %s %s %s %s %s %s",
				   argv[0], argv[1], argv[2], argv[3], argv[4],
				   argv[5], argv[6], argv[7], argv[8], argv[9] );

	if( argc < 1  ) return true;

	sol = false;
	sprintf( err, "Unknown command" );
	double auxsim, maxsim = 0;
	int maxi;
	
	//calling the appropiate function depending on the command
	for( i = 0 ; i < sizeof(commands)/ sizeof(command_t) ; i++ ){
		if( strncmp( argv[0], commands[i].com, strlen(commands[i].com) ) == 0
			&& !isalpha( argv[0][strlen(commands[i].com)] ) ){
			maxsim = 0;
			err[0] = 0;
			sol = commands[i].func( game, argc, argv, err );
			if( errorCode() != NOERROR ){
				sprintf( err, "%s", errorMessage( errorCode() ) );
				sol = false;
			}
			break;
		}
		if( ( auxsim = editDistance( argv[0], commands[i].com ) ) > maxsim ){
			maxsim = auxsim;
			maxi = i;
		}
	}
	if( maxsim >= MIN_SIMILARITY )
		sprintf( err+15, "\nDid you mean: \"%s\"", commands[maxi].com );

	freeMatrix( argv, MAX_ARGS );
	return sol;
}

/** *******************************************************************
****************************** COMMANDS *******************************
******************************************************************* **/


/**
* Checks if there is a valid path between two positions in the board
*
* @param game	contains every information needed on the current game
* @param x1		initial x coordenate of the path about to be checked
* @param y1		initial y coordenate of the path about to be checked
* @param x2		final x coordenate of the path about to be checked
* @param y2		final y coordenate of the path about to be checked
*
* @return 		true if there is a valid path, otherwise false
*/
bool areConnected( game_t * game, int x1, int y1, int x2, int y2 ){
	// BFS to find minimum path

	struct coord{
		int x,y;
	} move[4] = { {-1,0}, {0,1}, {1,0}, {0,-1} };

	struct node{
		int x, y;
	} queue[ game->players[ game->state.next ].board.emptySpots + 1 ];

	int read = -1, write = 0, x, y, i;

	bool touched[ game->options.height ][ game->options.width ];

	memset( &touched[0][0], false, sizeof(touched) * sizeof(bool) );
	queue[write++] = (struct node){x1,y1};

	while( ++read < write ){
		if( queue[read].x == x2 && queue[read].y == y2 )
			break;
		for( i = 0 ; i < sizeof(move)/sizeof(struct coord) ; i++ ){
			x = queue[read].x + move[i].x;
			y = queue[read].y + move[i].y;
			if( entre( 0, x, game->options.width )
				&& entre( 0, y, game->options.height )
				&& game->players[ game->state.next ].board.matrix[y][x] == 0
				&& !touched[y][x] ){
						touched[y][x] = true;
						queue[write++] = (struct node){x,y};
			}
		}
	}
	return read < write;
}

/**********************************************************************
****************************** movePiece ******************************
**********************************************************************/

/**
* Moves token from a position to another checking for winning and losing plays.
*
* @param game 	contain every information needed on the current game
* @param argc	quantity of items in argv
* @param argv   containins the parameters needed by the command
* @param err    its an output containing the type of error
*
* @return		true if the move could be made. otherwise, false
*/
bool movePiece( game_t * game, int argc, char ** argv, char * err ){
	int i;
	char s[ argc * MAX_ARGS ];

	s[0] = 0;
	//reading the command given in argv
	for( i = 0 ; i < argc ; i++ )
		strcat( s, argv[i] );
	
	int x1, y1, x2, y2;
	i = sscanf( s, "[%d,%d][%d,%d]", &y1, &x1, &y2, &x2 );
	//checking that the given command is valid
	if( i < 4 ){
		sprintf( err, "Format error:\n"
					"Must be: [ row_1, column_1 ][ row_2, column_2 ]" );
		return false;
	}
	if( ! entre( 0, y1, game->options.height ) ){
		sprintf( err, "Rank error:\nThe first row must belong to the "
					"interval [0,%d]", game->options.height - 1 );
		return false;
	}
	if( ! entre( 0, x1, game->options.width ) ){
		sprintf( err, "Rank error:\nThe first column must belong to the "
					"interval [0,%d]", game->options.width - 1 );
		return false;
	}
	if( ! entre( 0, y2, game->options.height ) ){
		sprintf( err, "Rank error:\nThe second row must belong to the "
					"interval [0,%d]", game->options.height - 1 );
		return false;
	}
	if( ! entre( 0, x2, game->options.width ) ){
		sprintf( err, "Rank error:\nThe second column must belong to the "
					"interval [0,%d]", game->options.width - 1 );
		return false;
	}
	if( game->players[ game->state.next ].board.matrix[y1][x1] == 0 ){
		sprintf( err, "The origin position must not be empty" );
		return false;
	}
	if( game->players[ game->state.next ].board.matrix[y2][x2] != 0 ){
		sprintf( err, "The target position must not be occupied" );
		return false;
	}
	if( !areConnected( game, x1, y1, x2, y2 ) ){
		sprintf( err, "There must be a path of unoccupied spaces from the "
					"origin position to the target position" );
		return false;
	}
	// maintain undo
	game->players[ game->state.next ].canUndo = true;
	
	copyMatrix( game->players[ game->state.next ].board.matrix,
				game->players[ game->state.next ].lastBoard. matrix,
				game->options.height, game->options.width );
	game->players[ game->state.next ].lastBoard.points =
								game->players[ game->state.next ].board.points;
	game->players[ game->state.next ].lastBoard.emptySpots =
							game->players[ game->state.next ].board.emptySpots;
	// maintain board
	game->players[ game->state.next ].board.matrix[y2][x2] =
					game->players[ game->state.next ].board.matrix[y1][x1];
	game->players[ game->state.next ].board.matrix[y1][x1] = 0;

	// if made a line, erase it and count points (winning play)
	if( ! winningPlay( game, game->state.next, x2, y2, true ) ){
		// else, randFill()
		randFill( game, game->state.next, game->options.tokensPerTurn, false );
		// change turn
		i = 0; 
		do{
			game->state.next++;
			game->state.next %= game->numPlayers;
			i++;
		}while( game->players[ game->state.next ].board.emptySpots <= 0 &&
					i <= game->numPlayers );
	}else
	// board is empty
	if( game->players[ game->state.next ].board.emptySpots >=
								game->options.width * game->options.height ){

		randFill( game, game->state.next, game->options.tokensPerTurn, true );
	}

// 		check for gameover
// 	if( i > game->numPlayers ||
// 		time(NULL) - game->state.lastTime >= game->state.timeLeft ){
// 			GAME OVER FOR EVERYONE
// 	}

	return true;
}

/**********************************************************************
******************************** save *********************************
**********************************************************************/

/**
* Saves the current state of the game.	
* 
* @param game 	contains every information needed on the current game
* @param argc	quantity of items in argv
* @param argv   contains the parameters needed by the command
* @param err    its an output string containing the type of error
* 
* @return		true if the game could be saved. otherwise, false
*/
bool save( game_t * game, int argc, char ** argv, char * err ){

	//checking that the given command in argv is valid
	if( argc > 2 || strcmp( "save", argv[0] ) != 0 ){
		sprintf( err, "Wrong usage\n"
		"Try 'save --help' for more information");
		return false;	
	}
	if( argc == 1 ){
		sprintf( err, "Missing file operand\n"
					"Try 'save --help' for more information");
		return false;
	}
	if( strcmp( argv[1], "--help" ) == 0 ){
		sprintf( err, "Usage: save filename\n"
					"Saves the current game to file 'filename'");
		return false;
	}
    //writing and saving the game
	writeGame( game, argv[1] );

	return true;
}

/**********************************************************************
******************************* buyItem *******************************
**********************************************************************/

/**
* Buys an item so that the player can have much more fun!	
* 
* @param game 	contains every information needed on the current game
* @param argc	quantity of items in argv
* @param argv   contains the parameters needed by the command
* @param err    its an output containing the type of error
* 
* @return		true if the item could be bought. otherwise, false
*/
bool buyItem( game_t * game, int argc, char ** argv, char * err ){
	//TODO
	return true;
}

/**********************************************************************
****************************** throwItem ******************************
**********************************************************************/

/**
* Throws the item! Total mayhem in the board!	
* 
* @param game 	contains every information needed on the current game
* @param argc	quantity of items in argv
* @param argv   a vector of strings containing the parameters needed by the command
* @param err    its an output containing the type of error
* 
* @return		true if the itme could be thrown. otherwise, false
*/
bool throwItem( game_t  * game, int argc, char ** argv, char * err ){
	//TODO
	return true;
}

/**********************************************************************
********************************* undo ********************************
**********************************************************************/

/**
* Undo the last move made.	
* 
* @param game 	contains every information needed on the current game
* @param argc	quantity of items in argv
* @param argv   contains the parameters needed by the command
* @param err    its an output containing the type of error
* 
* @return		true if the move could be undone. otherwise, false
*/
bool undo( game_t * game, int argc, char ** argv, char * err ){
	
	//checking that the given command in argv is valid
	if( argc == 2 && strcmp( argv[1], "--help" ) == 0 ){
		sprintf( err, "Usage: undo\n"
					"Undoes the last move\n"
					"It can only be used once some move has been done, "
					"and it can't be used two consecutive times" );
		return false;
	}
	if( argc > 1  || strcmp( "undo", argv[0] ) != 0 ){
		sprintf( err, "Wrong usage\n"
					"Try 'undo --help' for more information");
		return false;
	}
	//checking that the player can undo
	if( game->options.mode != SINGLEMODE ){
		sprintf( err, "'undo' command is only available in one player"
					", no time mode" );
		return false;
	}
	if( game->players[ game->state.next ].canUndo == false ){
		sprintf( err, "'undo' command cannot be used twice in a row or in "
					"the first turn" );
		return false;
	}
    
	game->players[ game->state.next ].canUndo = false;
	// swap boards;
	board_t aux = game->players[ game->state.next ].board;

	game->players[ game->state.next ].board =
									game->players[ game->state.next ].lastBoard;
	game->players[ game->state.next ].lastBoard = aux;

	return true;
}

/**********************************************************************
********************************* quit ********************************
**********************************************************************/

/**
* Quits the game.	
* 
* @param game 	contains every information needed on the current game
* @param argc	quantity of items in argv
* @param argv   contains the parameters needed by the command
* @param err    its an output containing the type of error
* 
* @return		true if the parameters were valid. otherwise, false
*/
bool quit( game_t * game, int argc, char ** argv, char * err ){
	//checking that the parameters in argv are valid
	if( argc == 2 && strcmp( argv[1], "--help" ) == 0 ){
		sprintf( err, "Usage: quit\n"
					"Quits the current game" );
		return false;
	}
	if( argc > 1 || strcmp( "quit", argv[0] ) != 0 ){
		sprintf( err, "Wrong usage\n"
			"Try 'quit --help' for more information");
		return false;
	}
    
	//returning the appropiate data so that the game is quitted
	err[0]=0;
	return false;
}

/**********************************************************************
********************************* help ********************************
**********************************************************************/


/**
* Prints help for the user.	
* 
* @param game 	contains every information needed on the current game
* @param argc	quantity of items in argv
* @param argv   contains the parameters needed by the command
* @param err    its an output containing the type of error
* 
* @return		true if the parameters were valid. otherwise, false
*/

bool help( game_t * game, int argc, char ** argv, char * err ){

	//checking the data given in argv
	if( argc == 2 && strcmp( argv[1], "--help" ) == 0 ){
		sprintf( err, "Usage: help\n"
					"Shows available commands" );
		return false;
	}
	if( argc > 1 || strcmp( "help", argv[0] ) != 0 ){
		sprintf( err, "Wrong usage\n"
		"Try 'help --help' for more information");
		return false;
	}
	//setting the return data so that it can be printed later
	sprintf( err, //"These commands are defined internally\n"
				"Type 'name --help' to find out more about the function 'name'"
				"\n \n"
				"  [row_1,column_1][row_2,column_2]\n"
				"  save filename\n"
				"  undo\n"
				"  quit\n"
				"  help\n"
				"  buy {item}  TODO\n"
				"  throw {item}  TODO\n" );
	return true;
}

/**********************************************************************
****************************** roflcopter *****************************
**********************************************************************/

/**
* Prints a ROFLcopter.	
* 
* @param game 	contains every information needed on the current game
* @param argc	quantity of items in argv
* @param argv   contains the parameters needed by the command
* @param err    its an output containing the type of error
* 
* @return		true if the parameters were valid. otherwise, false
*/

bool roflcopter( game_t * game, int argc, char ** argv, char * err ){
	sprintf( err,
#ifdef __unix__
			"\033[5mROFL:ROFL\033[25m:LOL:\033[5mROFL:ROFL\033[25m\n"
			"           |\n"
			" \033[5mL\033[25m   /---------\n"
			"\033[5mL\033[25mO\033[5mL\033[25m===       []\\\n"
			" \033[5mL\033[25m    \\         \\\n"
			"       \\_________\\\n"
			"         |     |\n"
			"      -------------/\n"
#else
			".............<ROFL ROFL ROFL ROFL>.\n"
			".......................| |.........\n"
			"................... __\\||/___......\n"
			".\\\\...............|'-|-|.\\\\...\\....\n"
			"..\\ \\_...........|--|--|..\\\\...\\...\n"
			"../ L \\______,/---------\\___\\___\\..\n"
			".|L O L|------------O------ ----,\\.\n"
			"..\\ L /________,---''----------, /.\n"
			"../ /...........\\____________ ,/...\n"
			".//............\\___//__/\\__\\\\__/...\n"
#endif
			);

	return false;
}