// errno2.c
#include "error.h"

static error nerror;

void clear_error(){
	nerror = NOERROR;
}

error error_code(){
	return nerror;
}

void raise_error(error num){
	nerror = num;
}

char * error_message(error num){
	switch(num){
		case NOERROR: 			return "No error";
		case ARITHMETICERROR: 	return "Arithmetic error";
		case MEMORYERROR: 		return "Memory error";
		case FILEERROR:			return "Error with file";
		case TIMEERROR:			return "Error while geting time";
		
		default: 				return "Unknown error";
	}
}