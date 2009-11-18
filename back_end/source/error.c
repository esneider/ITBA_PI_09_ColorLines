/**
* error.c
* Handles all type of errors during the game.
*/
#include "error.h"

static error nError;

/**
* Clears errors.
*
*/
void clearError(){
	nError = NOERROR;
}

/**
* Returns the current error code
*
* @return error code
*/
error errorCode(){
	return nError;
}

/**
* Raise an error
*
* @param num	error code
*/
void raiseError( error num ){
	nError = num;
}

/**
* Returns the error message corresponding to the given error code.
*
* @param  error	error code
*
* @return		error message
*/
char * errorMessage( error num ){
	switch(num){
		case NOERROR: 			return "No error";
		case ARITHMETICERROR: 	return "Arithmetic error";
		case MEMORYERROR: 		return "Memory error";
		case FILEERROR:			return "Error with file";
		case TIMEERROR:			return "Error while geting time";
		case INPUTERROR:		return "Error while reading input";
		case CORRUPTFILE:		return "Input file was corrupted";

		default: 				return "Unknown error";
	}
}