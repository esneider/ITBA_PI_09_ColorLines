// game.h
#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

typedef char ** board_t;

// GAME struct
typedef struct{
	board_t board;
	int points;
	bool canPlay;
	bool canUndo;
	board_t lastBoard;
} player_t;

typedef struct{
	clock_t lastTime;
	clock_t timeLeft; // seconds left (mode 1)
	int next; // next player (mode 2)
} state_t;

typedef struct{
	int mode; // 0 normal, 1 time, 2 two-players
	size_t height;
	size_t width;
	int num_colors;
	int tokens_per_line; // minimum number of consecutive pieces to consider it a line
	int tokens_per_turn; // new random pieces at each round
} options_t

typedef struct{
	player_t * players;
	state_t state;
} game;


// creates a new game
game * new_game( int mode, int time, int height, int width, int colors, int line_pieces, int round_pieces );

// writes game to file
void write_game( game * g, char * file );

// reads game from file
game * read_game(char * file);


#endif