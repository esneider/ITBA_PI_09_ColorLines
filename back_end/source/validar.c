// Funciones que verifican el modo de juego y las opciones ingresadas por el jugador
#include <string.h>
#include <stdlib.h>
#define MAX_X 500
#define MAX_Y 500

// Verifica modo
bool
validarModo(int modo){
	if (modo>=1 && modo<=5)
		return true;
	else
		return false;
}

// Verifica las dimensiones del tablero
bool
validarDim(int x, int y, char * error){
	if (x<5 || y<5){
		strcpy(error, "Ambas deben ser mayor a 5");
		return false;
	}
	// Tope es 500, ninguna dimension puede pasarlos
	else if(x>MAX_X || y>MAX_Y){
		strcpy(error, "Dimensiones demasiado grandes, ingrese nuevamente");
		return false;
	}
	else
		return true;
}

// Verifica cantidad de colores
bool
validarColores(int colores){
	if (colores>=2 && colores<= 9)
		return true;
	else
		return false;
}

// Verificas cantidad de bolillas que aparecen, que se agregan y que forman una linea
bool
validarBolillas(int bolillas, int x, int y, char * error){
	if(bolillas<=0){
		strcpy(error, "Tiene que ser mayor a 0");
		return false;
	}
	else if(bolillas>x*y){
		strcpy(error, "Demasiadas bolillas para la dimension del tablero");
		return false;
	}
	else
		return true;
}