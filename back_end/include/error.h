//error.h
#ifndef ERROR_H
#define ERROR_H

// if DEBUG is defined, raise_error_if prints in screen the error, file, function & line
#define DEBUG


// Error types
typedef enum{
	NOERROR,
	ARITHMETICERROR,
	MEMORYERROR,
	FILEERROR,
	TIMEERROR
} error;

// Quita errores pasados
void clear_error();

// Actual error code
error error_code();

// if "comp" is TRUE raise error "num" and return "ret"
#define raise_error_if( comp, num, ret ) { if(!(comp)){raise_error(num); return ret;} } 

// raise error "num"
void raise_error(error num);

// error message for error code "num"
char * error_message(error num);


#ifdef DEBUG
	#include <stdio.h>
	#undef raise_error_if
	#define raise_error_if( comp, num, ret )								\
		{ if (!(comp)){														\
			raise_error(num);												\
			printf( "In file %s\n%d :: %s => %s\nAsertion failed: %s\n\n",	\
					__FILE__,__LINE__,__func__,error_message(num),#comp),	\
			fflush(stdout);													\
			return ret;														\
		  } }
#endif

#endif