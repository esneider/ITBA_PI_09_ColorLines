/**
* @file error.c
* Error handling
*/

#include "error.h"

static error nError;


/**
* Clears errors.
*
* @see raiseError()
*/

void
clearError()
{
	nError = NOERROR;
}


/**
* Returns the current error code.
*
* @return error code
*
* @see raiseError()
*/

error
errorCode()
{
	return nError;
}


/**
* Raise an error.
*
* @param num	error code
*/

void
raiseError( error num )
{
	nError = num;
}


/**
* Returns the error message corresponding to the given error code.
*
* @param  num	error code
*
* @return error message
*
* @see raiseError();
*/

char *
errorMessage( error num )
{
	switch(num){
		case NOERROR:			return "No error";
		case ARITHMETICERROR:	return "Arithmetic error";
		case MEMORYERROR:		return "Memory error";
		case FILEERROR:			return "Error with file";
		case TIMEERROR:			return "Error while geting time";
		case INPUTERROR:		return "Error while reading input";
		case CORRUPTFILE:		return "Input file was corrupted";
		case COMPUTATIONALERROR:return "Computer power is not enough";

		default: 				return "Unknown error";
	}
}
