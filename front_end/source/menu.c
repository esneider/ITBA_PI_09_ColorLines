// Elegir el tipo de juego
#include <stdio.h>

#include "utils.h"
#include "menu.h"

typedef enum{
	MODE0 = SINGLEMODE,
	MODE1 = TIMEMODE,
	MODE2 = MULTIPLMODE,
	READFROMFILE,
	QUIT
} modeOptions_t;


// reads an integer and asserts it's in the interval [a,b]
static int askInt( const int a, const int b){
	int sol;
	static char error[MAX_ERR_LEN];

	error[0] = 0;
	do{
		printf( "%s", error );
		if( scanf( "%d", &sol ) != 1 ){
			sprintf( error, "Format error:\nMust be an integer\n");
			clearBuffer();
			continue;
		}
		clearBuffer();
	}while( !validateInt( a, sol, b+1, error ) );

	return sol;
}

// reads two integers and asserts they are in the intervals [a1,b1]
// and [a2,b2] respectively
static void ask2Int( const int a1, int * n1, const int b1,
					 const int a2, int * n2, const int b2 ){
	static char error[MAX_ERR_LEN];
	
	error[0] = 0;
	do{
		printf( " %s", error );
		if( scanf( " %d %d", n1, n2 ) != 2 ){
			sprintf( error, "Format error:\nMust be two integers, "
						"space separated\n");
			clearBuffer();
			continue;
		}
		clearBuffer();
	}while( !validateInt( a1, *n1, b1+1, error ) ||
			!validateInt( a2, *n2, b2+2, error ) );
}

// reads a string and asserts it's not empty
static char * askString( char * str ){
	do{
		if( scanf( " %s", str ) != 1 ){
			printf("Format error:\nMust be a string\n");
			clearBuffer();
			continue;
		}
	}while(0);
	return str;
}

// Choose game mode
static modeOptions_t chooseMode(){
	printf("Enter the game mode [1-5]:\n"
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

// Choose game options
static options_t chooseOptions( mode_t mode ){
	options_t options;
	int h, w;

	options.mode = mode;
	if( options.mode == TIMEMODE ){
		printf("Enter the time limit (in minutes):\n");
		options.initialSeconds = 60 * askInt(1, 10000);
		// you cannot play more than one week :) 7 * 24 * 60 = 10080
	}

	printf("Enter the dimensions of the board "
		"(rows and columns space separated):\n");
	ask2Int(5, &h, 500, 5, &w, 500);
	options.height = h;
	options.width = w;
	// I don't think you have a bigger monitor

	printf("Enter the number of colors with which you wish to play:\n");
	options.numColors = askInt(2, 9);

	printf("Enter the number of pieces that are initially on the board:\n");
	options.initialTokens = askInt(1, options.width * options.height );

	printf("Enter the number of pieces that make a line:\n");
	options.tokensPerLine = askInt(3, 700);
	// 500 * sqrt(2) = 707

	printf("Enter the number of pieces that are added on each turn:\n");
	options.tokensPerTurn = askInt(1, options.width * options.height );

	return options;
}

// Display game menu
game_t * menu(){
	
	options_t options;
	char str[MAX_COM_LEN];
	modeOptions_t mode = chooseMode();

	switch( mode ){
		case MODE0:
		case MODE1:
		case MODE2:
			options = chooseOptions( (mode_t)mode );
			return newGame( &options );
		case READFROMFILE:
			printf("Enter the name of the file:\n");
			return readGame( askString( str ) );
		case QUIT:
		default:
			return NULL;
	}
}