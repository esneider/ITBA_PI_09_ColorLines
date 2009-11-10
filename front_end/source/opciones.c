// Choose the type of game

#include <stdio.h>
#define MAX 100
#define CLEAR_BUFFER while(getchar() != '\n')
typedef enum {MOD0, MOD1, MOD2, REED, FINISH} t_mode;



// Choose game's mode
t_mode chooseMode(){
	t_mode mode;
	printf("Enter the mode of the game [1-5]: \n");
	printf("1. One player mode normal\n");
	printf("2. One player mode by time\n");
	printf("3. Two players mode normal\n");
	printf("4. Recover a saved game\n");
	printf("5. Exit game\n");
	scanf("%d", &mode);
	CLEAR_BUFFER;
	while(!validateMode(mode)){
		printf("Range error, must be between 1 and 5, enter again: ");
		scanf("%d", &mode);
		CLEAR_BUFFER;
	}
	return mode;
}

// Choose game's options
void chooseOptions(int time, t_mode mode ){
	int dimx, dimy, colors, inicialQuant, minBalls, addBalls;
	char error[MAX];
	printf("Enter the two dimensions of the board separated by space [min 5 5]: ");
	scanf("%d %d", &dimx, &dimy);
	CLEAR_BUFFER;
	while(!validateDim(dimx, dimy, error)){
		printf("%s: ", error);
		scanf("%d %d", &dimx, &dimy);
		CLEAR_BUFFER;
	}
	printf("Enter the numer of colors you want to play with [between 2 and 9]: ");
	scanf("%d", &colors);
	CLEAR_BUFFER;
	while(!validateColors(colors)){
		printf("Range error, must be between 2 and 9, enter again: ");
		scanf("%d", &colors);
		CLEAR_BUFFER;
	}
	printf("Enter the number of balls that are initially on the board: ");
	scanf("%d", &inicialQuant);
	CLEAR_BUFFER;
	while(!validateBalls(inicialQuant, dimx, dimy, error)){
		printf("%s: ", error);
		scanf("%d", &inicialQuant);
		CLEAR_BUFFER;
	}
	printf("Enter de amount of balls that form a line: ");
	scanf("%d", &minBalls);
	CLEAR_BUFFER;
	while(!validateBalls(minBalls, dimx, dimy, error)){
		printf("%s: ", error);
		scanf("%d", &minBalls);
		CLEAR_BUFFER;
	}
	printf("Enter de amount of balls that are added at each turn: ");
	scanf("%d", &addBalls);
	CLEAR_BUFFER;
	while(!validateBalls(addBalls, dimx, dimy, error)){
		printf("%s: ", error);
		scanf("%d", &addBalls);
		CLEAR_BUFFER;
	}
	// falta pasar la cantidad de bolillas iniciales
	return new_game(mode, time, dimy, dimx, colors, minBalls, addBalls);
}