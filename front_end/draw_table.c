#include <stdio.h>
#include <stdlib.h>
#include "colors.h"

void
draw_table(char ** table, int width, int height, int startx, int starty){
    int i, j;
	static int colores[] = {GRIS,ROJO,AZUL,VERDE,AMARILLO,CELESTE,ROSA,VIOLETA,MARRON,VERDE_CLARO};
    backcolor(NEGRO);
	textcolor(BLANCO);
	for(i=0; i<starty; i++)
        printf("\n\n");
    for(i=0; i<startx;i++)
        printf("    ");
    printf(" ");
    for( i=0 ; i<width-startx ; i++ )
        printf(" - -");
    printf(" \n");
    for( i=0 ; i<height-starty ; i++ ){
        for(j=0; j<startx; j++)
            printf("    ");
        for( j=0 ; j< width - startx; j++ ){
			printf(" | ");
			textcolor( colores[ (int)table[i][j] ] );
            printf("%d", (int)table[i][j]);
			textcolor(BLANCO);
		}
		printf(" |\n ");
        for(j=0; j<startx;j++)
            printf("    ");
        for(j=0; j<width-startx; j++)
            printf(" - -");
        printf(" \n");
    }
	textattr(CLEAR);
	printf("\n");
}

int main(){
	char ** mat = malloc(10 * sizeof(char*));
	for(int i=0;i<10;i++){
		mat[i] = malloc(10*sizeof(char));
		for(int j=0;j<10;j++)
			mat[i][j] = rand()%10;
	}
	draw_table( mat, 10, 10, 0, 0 );
}