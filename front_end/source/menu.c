// menu.c
#include <stdio.h>
#include "error.h"
#include "utils.h"
#include "defines.h"
#include "userInterface.h"
#include "menu.h"

typedef enum{
	MODE0 = SINGLEMODE,
	MODE1 = TIMEMODE,
	MODE2 = MULTIPLMODE,
	READFROMFILE,
	QUIT
} modeOption_t;

/**
* Reads an integer and asserts it's in the interval [a,b].
*
* @param a	lower bound
* @param b 	upper bound
*
* @return 	integer between a and b
*/
static int askInt( const int a, const int b ){
	int sol;
	char c;
	static char error[MAX_ERR_LEN];
	static char buffer[MAX_COM_LEN];

	error[0] = 0;
	do{
		clearScreen();
		drawText( NULL );
		drawPanel( error );
		askCommand( buffer );
		raiseErrorIf( errorCode() == NOERROR, errorCode(), -1 );
		error[0] = 0;
		if ( ( c = sscanf( buffer, " %d %c ", &sol, &c ) ) != 1 && buffer[0] != '\n' )
			sprintf( error, "Format error:\nMust be an integer" );
	}while( c != 1 || !validateInt( a, sol, b+1, error ) );

	return sol;
}

/**
* Reads two integers and asserts they are in the intervals [a1,b1] and [a2,b2]
* respectively.
*
* @param a1 	lower bound for the first integer
* @param *n1 	pointer to first element
* @param b1 	upper bound for the first integer
* @param a2 	lower bound for the second integer
* @param *n2 	pointer to the second element
* @param b2 	upper bound for the second integer
*/

static void ask2Int( const int a1, int * n1, const int b1,
					 const int a2, int * n2, const int b2 ){
	char c;
	static char error[MAX_ERR_LEN];
	static char buffer[MAX_COM_LEN];

	error[0] = 0;
	do{
		clearScreen();
		drawText( NULL );
		drawPanel( error );
		askCommand( buffer );
		raiseErrorIf( errorCode() == NOERROR, errorCode(), );
		error[0] = 0;
		if( ( c = sscanf( buffer, " %d %d %c", n1, n2, &c ) ) != 2 && buffer[0] != '\n' )
			sprintf( error, "Format error:\nMust be two integers, "
						"space separated" );
	}while( c != 2 || !validateInt( a1, *n1, b1+1, error ) ||
						!validateInt( a2, *n2, b2+1, error ) );
}

/**
* Reads a string and asserts it's not empty.
*
* @param str 	empty string
*
* @return string with the filename
*/

static char * askString( char * str ){
	char c;
	static char error[MAX_ERR_LEN];
	static char buffer[MAX_COM_LEN];

	do{
		clearScreen();
		drawText( NULL );
		drawPanel( error );
		askCommand( buffer );
		raiseErrorIf( errorCode() == NOERROR, errorCode(), NULL );
		error[0] = 0;
		if( ( c = sscanf( buffer, " %s", str ) ) != 1 && buffer[0] != '\n' )
			sprintf( error, "Format error:\nMust be a string" );
	}while( c != 1 );
	return str;
}

/**
* Choose game mode. Use {@link askInt{1, 5}} to ask for an integer.
*
* @param
*
* @return	game mode
*/
 
static modeOption_t chooseMode(){
	drawText("Enter the game mode [1-5]:\n"
			"  1. Single player normal mode\n"
			"  2. Single player time mode\n"
			"  3. Two players\n"
			"  4. Recover game from file\n"
			"  5. Quit\n" );

	switch( askInt(1,5) ){
		case 1: return MODE0;
		case 2: return MODE1;
		case 3: return MODE2;
		case 4: return READFROMFILE;
		case 5:
		default: return QUIT;
	}
}

/**
* Choose game options. Use {@link ask2Int} for dimensions of the matrix. 
* Use {@link askInt} for other game options.
*
* @param mode 	game mode
*
* @return 		structure containing the options chosen.
*/

static options_t chooseOptions( modus_t mode ){
	options_t options;
	int h, w;

	options.mode = mode;
	if( options.mode == TIMEMODE ){
		drawText("Enter the time limit (in minutes):\n");
		options.initialSeconds = 60 * askInt( 1, MAX_MINUTES );
		raiseErrorIf( errorCode() == NOERROR, errorCode(), options );
	}

	drawText("Enter the dimensions of the board "
			"(rows and columns space separated):\n");
	ask2Int( MIN_TAB_DIM, &h, MAX_TAB_DIM, MIN_TAB_DIM, &w, MAX_TAB_DIM );
	raiseErrorIf( errorCode() == NOERROR, errorCode(), options );

	options.height = h;
	options.width = w;

	drawText("Enter the number of colors with which you wish to play:\n");
	options.numColors = askInt( MIN_COLORS, MAX_COLORS );
	raiseErrorIf( errorCode() == NOERROR, errorCode(), options );

	drawText("Enter the number of pieces that are initially on the board:\n");
	options.initialTokens = askInt(1, options.width * options.height );
	raiseErrorIf( errorCode() == NOERROR, errorCode(), options );

	drawText("Enter the number of pieces that make a line:\n");
	options.tokensPerLine = askInt( MIN_TOK_PER_LINE,
										min( options.width, options.height ) );
	raiseErrorIf( errorCode() == NOERROR, errorCode(), options );

	drawText("Enter the number of pieces that are added on each turn:\n");
	options.tokensPerTurn = askInt(1, options.width * options.height );
	raiseErrorIf( errorCode() == NOERROR, errorCode(), options );

	return options;
}

/**
* Display game menu. Use {@link chooseMode},  for modes 1, 2 and 3 
* use {@link chooseOptions} and {@link newGame}, for mode 4 
* use {@link askString} and {@link readGame}.
*
* @param

* @return 	If mode is Quit Null, else a pointer to the 
* 			main structure of the game.
*/

game_t * menu(){

	options_t options;
	char str[MAX_COM_LEN];
	modeOption_t modeOption = chooseMode();
	raiseErrorIf( errorCode() == NOERROR, errorCode(), NULL );

	switch( modeOption ){
		case MODE0:
		case MODE1:
		case MODE2:
			options = chooseOptions( (modus_t)modeOption );
			raiseErrorIf( errorCode() == NOERROR, errorCode(), NULL );
			return newGame( &options );
		case READFROMFILE:
			drawText("Enter the name of the file:\n");
			return readGame( askString( str ) );
		case QUIT:
		default:
			return NULL;
	}
}