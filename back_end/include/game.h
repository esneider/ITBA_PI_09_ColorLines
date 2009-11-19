/**
* @file game.h
* Operations for the struct @c game
*/

#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <time.h>


typedef enum{
	SINGLEMODE, TIMEMODE, MULTIPLMODE
} modus_t; // mode_t is already defined in some compilers libraries

typedef struct{
	char ** matrix;
	int points;
	int emptySpots;
} board_t;

typedef struct{
	board_t board;
	board_t lastBoard;
	bool canUndo;
} player_t;

typedef struct{
	time_t lastTime;
	time_t timeLeft; // seconds left (mode 1)
	int next; // next player (mode 2)
} state_t;

typedef struct{
	modus_t mode; // 0 normal, 1 time, 2 two-players
	size_t height;
	size_t width;
	int numColors;
	int tokensPerLine; // minimum number of consecutive pieces to make a line
	int tokensPerTurn; // new random pieces at each round
	int initialTokens; // tokens located at random when starting
	time_t initialSeconds; // seconds before game is over
} options_t;

typedef struct{
	int numPlayers;
	options_t  options;
	state_t    state;
	player_t * players;
} game_t;


// creates a new game
game_t * newGame( options_t * options );

// frees allocated space for game
void freeGame( game_t * game );

// writes game to file
void writeGame( game_t * game, const char * file );

// reads game from file
game_t * readGame( const char * file );


#endif // GAME_H