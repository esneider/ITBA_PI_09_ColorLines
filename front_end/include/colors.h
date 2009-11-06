// colors.h
#ifndef COLORS_H
#define COLORS_H


typedef enum color{
	NEGRO        ,
	ROJO         ,
	VERDE        ,
	MARRON       ,
	AZUL         ,
	VIOLETA      ,
	CELESTE      ,
	GRIS_CLARO   ,
	GRIS         ,
	ROSA         ,
	VERDE_CLARO  ,
	AMARILLO     ,
	AZUL_CLARO   ,
	VIOLETA_CLARO,
	CELESTE_CLARO,
	BLANCO
	
} color;

typedef enum attr{
	CLEAR        ,
	NONE         ,
	
	NEGRITA      ,
	SUBRAYADO    ,
	BLINK        ,
	INVERTIDO    ,
	
	NO_NEGRITA   ,
	NO_SUBRAYADO ,
	NO_BLINK     ,
	NO_INVERTIDO
	
} attr;

void textcolor( color c );

void backcolor( color c );

void textattr( attr a );


#endif