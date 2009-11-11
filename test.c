#include <stdio.h>
#include <string.h>
#include "game.h"
#include "commands.h"

int main(){
	char  err[1000];
	char  str[1000];
	options_t options;
	game_t * game = newGame( &options );
	printf("> ");
	gets( str );
// 	scanf(" %s", str);
	newCommand( game, str,err );
	printf( "%s\n", err );
// 	int x1, y1, x2, y2;
// 	int argc = scanf( " [ %d , %d ] [ %d , %d ]", &y1, &x1, &y2, &x2 );
// 	printf("%d",argc);
/*	printf( "%d\n", scanf("{%*s") );
	printf( "%d\n", strncmp("{dsad","{",1));*/
}