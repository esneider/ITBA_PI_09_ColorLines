/**
* @file command.h
* Command handling
*/

#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdbool.h>
#include "game.h"


bool newCommand( game_t * game, const char * s, char * msg );


#endif // COMMANDS_H