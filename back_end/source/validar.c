`// Verifies the mode and the options of the game
#include <string.h>
#include <stdbool.h>
#define MIN_MODE 1
#define MAX_MODE 5

// Verifies mode
bool validateMode(int mode){
	if (mode>=MIN_MODE && mode<=MAX_MODE)
		return true;
	else
		return false;
}

// Verifies the dimensions of the board
bool validateDim(int x, int y, char * error){
	if (x<MIN_DIM || y<MIN_DIM){
		strcpy(error, "Both must be greater than 5, enter again");
		return false;
	}
	// The maximum is 500, you can't overcome any dimension
	else if(x>MAX_DIM || y>MAX_DIM){
		strcpy(error, "Dimensions to large, enter again");
		return false;
	}
	else
		return true;
}

// Check colors
bool validateColors(int colors){
	if (colors>=MIN_COLORS && colors<= MAX_COLORS)
		return true;
	else
		return false;
}

// Check number of balls that appear, to be aggregated and forming a line
bool validateBalls(int balls, int x, int y, char * error){
	if(balls<MIN_BALLS){
		strcpy(error, "It must be greater than 1, enter again");
		return false;
	}
	else if(balls>x*y){
		strcpy(error, "Too many balls for the dimension of the board, enter again");
		return false;
	}
	else
		return true;
}