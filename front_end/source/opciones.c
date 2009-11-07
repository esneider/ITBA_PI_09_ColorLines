// Elegir el tipo de juego

#include <stdio.h>
#define MAX 100
#define CLEAR_BUFFER while(getchar() != '\n')
typedef enum {MOD0, MOD1, MOD2, REED, FINISH} t_mode;



// Elegir el modo de juego
t_mode
elegirModo(){
	t_mode modo;
	printf("Ingrese la modalidad del juego [1-5]: \n");
	printf("1. Un jugador modalidad nornal\n");
	printf("2. Un jugador modalidad por tiempo\n");
	printf("3. Dos jugadores modalidad normal\n");
	printf("4. Recuperar un juego guardado\n");
	printf("5. Terminar\n");
	scanf("%d", &modo);
	CLEAR_BUFFER;
	while(!validarModo(modo)){
		printf("Debe ser un numero entre 1 y 5: ");
		scanf("%d", &modo);
		CLEAR_BUFFER;
	}
	return modo;
}

// Elegir opciones de juego
void
elegirOpciones(int tiempo, t_mode modo ){
	int dimx, dimy, colores, cantinicial, minbolillas, addbolillas;
	char error[MAX];
	printf("Ingrese las dos dimensiones del tablero separadas por espacio [min 5 5]: ");
	scanf("%d %d", &dimx, &dimy);
	CLEAR_BUFFER;
	while(!validarDim(dimx, dimy, error)){
		printf("%s: ", error);
		scanf("%d %d", &dimx, &dimy);
		CLEAR_BUFFER;
	}
	printf("Ingrese la cantidad de colores con la que desea jugar [entre 2 y 9]: ");
	scanf("%d", &colores);
	CLEAR_BUFFER;
	while(!validarColores(colores)){
		printf("Error de rango, debe estar entre 2 y 9: ");
		scanf("%d", &colores);
		CLEAR_BUFFER;
	}
	printf("Ingrese la cantidad de bolillas que hay inicialmente en el tablero: ");
	scanf("%d", &cantinicial);
	CLEAR_BUFFER;
	while(!validarBolillas(cantinicial, dimx, dimy, error)){
		printf("%s: ", error);
		scanf("%d", &cantinicial);
		CLEAR_BUFFER;
	}
	printf("Ingresar la cantidad de bolillas que hacen una linea: ");
	scanf("%d", &minbolillas);
	CLEAR_BUFFER;
	while(!validarBolillas(minbolillas, dimx, dimy, error)){
		printf("%s: ", error);
		scanf("%d", &minbolillas);
		CLEAR_BUFFER;
	}
	printf("Ingresar la cantidad de bolillas que se agregan en cada turno: ");
	scanf("%d", &addbolillas);
	CLEAR_BUFFER;
	while(!validarBolillas(addbolillas, dimx, dimy, error)){
		printf("%s: ", error);
		scanf("%d", &addbolillas);
		CLEAR_BUFFER;
	}
	// falta pasar la cantidad de bolillas iniciales
	return new_game(modo, tiempo, dimy, dimx, colores, minbolillas, addbolillas);
}