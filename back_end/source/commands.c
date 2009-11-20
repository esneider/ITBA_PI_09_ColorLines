/**
* @file command.c
* Command handling
*/

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

bool movePiece ( game_t * game, int argc, char ** argv, char * msg );
bool save      ( game_t * game, int argc, char ** argv, char * msg );
bool buyItem   ( game_t * game, int argc, char ** argv, char * msg );
bool throwItem ( game_t * game, int argc, char ** argv, char * msg );
bool undo      ( game_t * game, int argc, char ** argv, char * msg );
bool quit      ( game_t * game, int argc, char ** argv, char * msg );
bool help      ( game_t * game, int argc, char ** argv, char * msg );
bool roflcopter( game_t * game, int argc, char ** argv, char * msg );

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
* Parses the command and calls the corresponding function.
*
* @param game		contains all information about current game
* @param s			containins the command line about to be processed
* @param[out] msg	its an output containing the type of error
*
* @return false if there is an error, otherwise true
*
* @see editDistance()
* @see movePiece()
* @see save()
* @see buyItem()
* @see throwItem()
* @see undo()
* @see quit()
* @see help()
* @see ROFLcopter()
*/

bool
newCommand( game_t * game, const char * s, char * msg )
{
	int i;
	bool sol;
	int argc;
	char ** argv;

	if( errorCode() != NOERROR ){
		sprintf( msg, "%s", errorMessage( errorCode() ) );
		return false;
	}

	argv = newMatrix( MAX_ARGS, MAX_COM_LEN );

	if( errorCode() != NOERROR ){
		sprintf( msg, "%s", errorMessage( errorCode() ) );
		return false;
	}

	msg[0] = 0;
	// parse the command
	argc = sscanf( s, "%s %s %s %s %s %s %s %s %s %s",
				   argv[0], argv[1], argv[2], argv[3], argv[4],
				   argv[5], argv[6], argv[7], argv[8], argv[9] );

	if( argc < 1  ){
		freeMatrix( argv, MAX_ARGS );
		return true;
	}

	sol = false;
	sprintf( msg, "Unknown command" );
	double auxsim, maxsim = 0;
	int maxi = 0;

	//call the appropiate function
	for( i = 0 ; i < sizeof(commands)/ sizeof(command_t) ; i++ ){
		if( strncmp( argv[0], commands[i].com, strlen(commands[i].com) ) == 0
							&& !isalpha( argv[0][strlen(commands[i].com)] ) ){
			maxsim = 0;
			msg[0] = 0;
			sol = commands[i].func( game, argc, argv, msg );
			if( errorCode() != NOERROR ){
				sprintf( msg, "%s", errorMessage( errorCode() ) );
				sol = false;
			}
			break;
		}
		// check for command similarity
		if( ( auxsim = editDistance( argv[0], commands[i].com ) ) > maxsim ){
			maxsim = auxsim;
			maxi = i;
		}
	}
	// if not succesful and there was a command similar enough
	if( maxsim >= MIN_SIMILARITY )
		sprintf( msg+15, "\nDid you mean: \"%s\"", commands[maxi].com );

	freeMatrix( argv, MAX_ARGS );

	if( errorCode() != NOERROR ){
		sprintf( msg, "%s", errorMessage( errorCode() ) );
		sol = false;
	}

	return sol;
}


/**
* Checks if there is a valid path between (@a x1,@a y1) and (@a x2,@a y2)
* in the board.
*
* @param game	contains all information about current game
* @param x1		initial x coordenate
* @param y1		initial y coordenate
* @param x2		final x coordenate
* @param y2		final y coordenate
*
* @return true if there is a valid path, otherwise false
*/

bool
areConnected( game_t * game, int x1, int y1, int x2, int y2 )
{
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


/**
* Moves token from a position to another checking for winning plays (lines).
*
* @param game		contains all information about current game
* @param argc		size of @a argv
* @param argv		parameters followinf the command
* @param[out] msg	output with message to write in the panel
*
* @return false if some message is to be written
*
* @see areConected()
* @see randFill()
* @see winningPlay()
*/

bool
movePiece( game_t * game, int argc, char ** argv, char * msg )
{
	int i;
	char s[ argc * MAX_ARGS ];

	s[0] = 0;
	for( i = 0 ; i < argc ; i++ )
		strcat( s, argv[i] );

	int x1, y1, x2, y2;
	i = sscanf( s, "[%d,%d][%d,%d]", &y1, &x1, &y2, &x2 );
	if( i < 4 ){
		sprintf( msg, "Format error:\n"
					"Must be: [ row_1, column_1 ][ row_2, column_2 ]" );
		return false;
	}
	if( ! entre( 0, y1, game->options.height ) ){
		sprintf( msg, "Rank error:\nThe first row must belong to the "
					"interval [0,%d]", game->options.height - 1 );
		return false;
	}
	if( ! entre( 0, x1, game->options.width ) ){
		sprintf( msg, "Rank error:\nThe first column must belong to the "
					"interval [0,%d]", game->options.width - 1 );
		return false;
	}
	if( ! entre( 0, y2, game->options.height ) ){
		sprintf( msg, "Rank error:\nThe second row must belong to the "
					"interval [0,%d]", game->options.height - 1 );
		return false;
	}
	if( ! entre( 0, x2, game->options.width ) ){
		sprintf( msg, "Rank error:\nThe second column must belong to the "
					"interval [0,%d]", game->options.width - 1 );
		return false;
	}
	if( game->players[ game->state.next ].board.matrix[y1][x1] == 0 ){
		sprintf( msg, "The origin position must not be empty" );
		return false;
	}
	if( game->players[ game->state.next ].board.matrix[y2][x2] != 0 ){
		sprintf( msg, "The target position must not be occupied" );
		return false;
	}
	if( !areConnected( game, x1, y1, x2, y2 ) ){
		sprintf( msg, "There must be a path of unoccupied spaces from the "
					"origin position to the target position" );
		return false;
	}
	// maintain undo
	game->players[ game->state.next ].canUndo = true;

	copyMatrix( game->players[ game->state.next ].lastBoard. matrix,
				game->players[ game->state.next ].board.matrix,
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
		if( errorCode() != NOERROR ){
			sprintf( msg, "%s", errorMessage( errorCode() ) );
			return false;
		}
	}

	return true;
}


/**
* Saves current game to file.
*
* @param game		contains all information about current game
* @param argc		size of @a argv
* @param argv		parameters followinf the command
* @param[out] msg	output with message to write in the panel
*
* @return false if some message is to be written
*
* @see writeGame()
*/

bool
save( game_t * game, int argc, char ** argv, char * msg )
{
	if( argc > 2 || strcmp( "save", argv[0] ) != 0 ){
		sprintf( msg, "Wrong usage\n"
		"Try 'save --help' for more information");
		return false;
	}
	if( argc == 1 ){
		sprintf( msg, "Missing file operand\n"
					"Try 'save --help' for more information");
		return false;
	}
	if( strcmp( argv[1], "--help" ) == 0 ){
		sprintf( msg, "Usage: save filename\n"
					"Saves the current game to file 'filename'");
		return false;
	}
	writeGame( game, argv[1] );

	if( errorCode() != NOERROR ){
		sprintf( msg, "%s", errorMessage( errorCode() ) );
		return false;
	}

	return true;
}


/**
* Buys an item so that the player can have much more fun!
*
* @param game		contains all information about current game
* @param argc		size of @a argv
* @param argv		parameters followinf the command
* @param[out] msg	output with message to write in the panel
*
* @return false if some message is to be written
*
* @see throwItem()
*/

bool
buyItem( game_t * game, int argc, char ** argv, char * msg )
{
	//TODO
	return true;
}


/**
* Throws the item! Total mayhem in the board!
*
* @param game		contains all information about current game
* @param argc		size of @a argv
* @param argv		parameters followinf the command
* @param[out] msg	output with message to write in the panel
*
* @return false if some message is to be written
*
* @see buyItem()
*/

bool
throwItem( game_t  * game, int argc, char ** argv, char * msg )
{
	//TODO
	return true;
}


/**
* Undoes the last move made.
*
* @param game		contains all information about current game
* @param argc		size of @a argv
* @param argv		parameters followinf the command
* @param[out] msg	output with message to write in the panel
*
* @return false if some message is to be written
*/

bool
undo( game_t * game, int argc, char ** argv, char * msg )
{
	if( argc == 2 && strcmp( argv[1], "--help" ) == 0 ){
		sprintf( msg, "Usage: undo\n"
					"Undoes the last move\n"
					"It can only be used once some move has been done, "
					"and it can't be used two consecutive times" );
		return false;
	}
	if( argc > 1  || strcmp( "undo", argv[0] ) != 0 ){
		sprintf( msg, "Wrong usage\n"
					"Try 'undo --help' for more information");
		return false;
	}
	if( game->options.mode != SINGLEMODE ){
		sprintf( msg, "'undo' command is only available in one player"
					", no time mode" );
		return false;
	}
	if( game->players[ game->state.next ].canUndo == false ){
		sprintf( msg, "'undo' command cannot be used twice in a row or in "
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


/**
* Quits the game.
*
* @param game		contains all information about current game
* @param argc		size of @a argv
* @param argv		parameters followinf the command
* @param[out] msg	output with message to write in the panel
*
* @return false if some message is to be written
*/

bool
quit( game_t * game, int argc, char ** argv, char * msg )
{
	if( argc == 2 && strcmp( argv[1], "--help" ) == 0 ){
		sprintf( msg, "Usage: quit\n"
					"Quits the current game" );
		return false;
	}
	if( argc > 1 || strcmp( "quit", argv[0] ) != 0 ){
		sprintf( msg, "Wrong usage\n"
			"Try 'quit --help' for more information");
		return false;
	}
	game->state.quit = true;
	return true;
}


/**
* Prints help for the user. Secret commands are not shown.
*
* @param game		contains all information about current game
* @param argc		size of @a argv
* @param argv		parameters followinf the command
* @param[out] msg	output with message to write in the panel
*
* @return false if some message is to be written
*/

bool
help( game_t * game, int argc, char ** argv, char * msg )
{
	if( argc == 2 && strcmp( argv[1], "--help" ) == 0 ){
		sprintf( msg, "Usage: help\n"
					"Shows available commands" );
		return false;
	}
	if( argc > 1 || strcmp( "help", argv[0] ) != 0 ){
		sprintf( msg, "Wrong usage\n"
		"Try 'help --help' for more information");
		return false;
	}
	sprintf( msg, //"These commands are defined internally\n"
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


/**
* Secret command.
* Prints a ROFLcopter.
*
* @param game		contains all information about current game
* @param argc		size of @a argv
* @param argv		parameters followinf the command
* @param[out] msg	output with message to write in the panel
*
* @return false if some message is to be written
*/

bool
roflcopter( game_t * game, int argc, char ** argv, char * msg )
{
	sprintf( msg,
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
