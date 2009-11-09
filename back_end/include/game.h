// game.h
#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

typedef char ** board_t;

typedef enum{
	SINGLEMODE, TIMEMODE, MULTIPLMODE
} mode_t;

// GAME struct
typedef struct{
	board_t board;
	board_t lastBoard;
	int points;
	int emptySpots;
// 	bool canPlay;
	bool canUndo;
} player_t;

typedef struct{
	clock_t lastTime;
	clock_t timeLeft; // seconds left (mode 1)
	int next; // next player (mode 2)
} state_t;

typedef struct{
	mode_t mode; // 0 normal, 1 time, 2 two-players
	size_t height;
	size_t width;
	int numColors;
	int tokensPerLine; // minimum number of consecutive pieces to consider it a line
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

// writes game to file
void writeGame( game_t * game, char * file );

// reads game from file
game_t * readGame( char * file );


#endif