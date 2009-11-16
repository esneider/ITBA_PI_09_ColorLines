// error.c
#include "error.h"

static error nError;

void clearError(){
	nError = NOERROR;
}

error errorCode(){
	return nError;
}

void raiseError( error num ){
	nError = num;
}

char * errorMessage( error num ){
	switch(num){
		case NOERROR: 			return "No error";
		case ARITHMETICERROR: 	return "Arithmetic error";
		case MEMORYERROR: 		return "Memory error";
		case FILEERROR:			return "Error with file";
		case TIMEERROR:			return "Error while geting time";
		case INPUTERROR:		return "Error while reading input";

		default: 				return "Unknown error";
	}
}