// game.c
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "error.h"
#include "game.h"

// creates a new matrix of height x width and 0es it
char ** new_matrix( int height, int width ){
	int i;
	char ** sol = malloc(height * sizeof(char*));
	raise_error_if(sol,MEMORYERROR,NULL);
	
	for(i = 0 ; i < height ; i++){
		sol[i] = calloc(width, sizeof(char));
		raise_error_if(sol,MEMORYERROR,NULL);
		// TODO : free unused memory (in case of error)
	}
	return sol;
}


// creates a new game
game * new_game( int mode, int time, int height, int width, int colors, int line_pieces, int round_pieces ){

	game * sol = malloc(sizeof(game));
	raise_error_if(sol,MEMORYERROR,NULL);
	
	sol->mode = mode;
	sol->height = height;
	sol->width = width;
	sol->colors = colors;
	sol->line_pieces = line_pieces;
	sol->round_pieces = round_pieces;
	sol->p1points = 0;
	sol->p1 = new_matrix(height, width);
	raise_error_if(sol->p1,error_code(),NULL);
	
	sol->p2points = -1;
	if(mode == 1){
		sol->un.time = time;
	}else{
		sol->un.next = 1;
		sol->p2points = 0;
		sol->p2 = new_matrix(height, width);
		raise_error_if(sol->p2,error_code(),NULL);
	}

	return sol;
}


void write_game( game * G, char * file ){
	int i;
	FILE * out = fopen(file, "wb");
	raise_error_if(out,FILEERROR,);
	
	// mode
	fwrite(&(G->mode), sizeof(int), 1, out);
	// un
	if(G->mode != 0)
		fwrite(&(G->un.time), sizeof(int), 1, out);
	// others
	fwrite(&(G->height), sizeof(int), 6, out);
	// player 1 board
	for(i = 0 ; i < G->height ; i++)
		fwrite(G->p1[i], sizeof(char), G->width, out);
	// player 2 board
	if(G->mode == 2){
		fwrite(&(G->p2points), sizeof(int), 1, out);
		for(i = 0 ; i < G->height ; i++)
			fwrite(G->p2[i], sizeof(char), G->width, out);
	}

	i = fclose(out);
	raise_error_if(i==0,FILEERROR,);
}

game * read_game(char * file){
	int i;
	FILE * in = fopen(file,"rb");
	raise_error_if(in,FILEERROR,NULL);

	game * sol = malloc(sizeof(game));
	raise_error_if(sol,MEMORYERROR,NULL);

	// mode
	fread(&(sol->mode), sizeof(int), 1, in);
	// un
	if(sol->mode != 0)
		fread(&(sol->un.time), sizeof(int), 1, in);
	// others
	fread(&(sol->height), sizeof(int), 6, in);
	// player 1 board
	sol->p1 = new_matrix(sol->height, sol->width);
	raise_error_if(sol->p1,error_code(),NULL);

	for(i = 0 ; i < sol->height ; i++)
		fread(sol->p1[i], sizeof(char), sol->width, in);
	// player 2 board
	if(sol->mode == 2){
		sol->p2 = new_matrix(sol->height, sol->width);
		raise_error_if(sol->p2,error_code(),NULL);

		fread(&(sol->p2points), sizeof(int), 1, in);
		for(i = 0 ; i < sol->height ; i++)
			fread(sol->p2[i], sizeof(char), sol->width, in);
	}

	i = fclose(in);
	raise_error_if(i==0,FILEERROR,NULL);
	
	return sol;
}