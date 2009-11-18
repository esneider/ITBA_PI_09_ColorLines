// error.c
#include "error.h"

static error nError;
/**
* Sets the variable nError as there is no error.
*
* @return void
*/
void clearError(){
	nError = NOERROR;
}

/**
* Returns the kind of error so that other functions can handle it.
*
* @return error depending on the kind of error
*/
error errorCode(){
	return nError;
}
/**
* Sets the variable nError as the according error.
*
* @param num	represents the kind of error
* @return void
*/
void raiseError( error num ){
	nError = num;
}

/**
* Returns the appropieate message depending on the kind of error.
*
* @param  error	represents the kind of error	
* @return error depending on the kind of error
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