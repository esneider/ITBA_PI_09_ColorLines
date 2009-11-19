/**
* @file error.h
* Error handling
*/

#ifndef ERROR_H
#define ERROR_H

// if DEBUG is defined, raise_error_if prints in screen the error, file,
// function & line
// #define DEBUG


// Error types
typedef enum{
	NOERROR,
	ARITHMETICERROR,
	MEMORYERROR,
	FILEERROR,
	TIMEERROR,
	INPUTERROR,
	CORRUPTFILE,
	COMPUTATIONALERROR
} error;

void clearError();

error errorCode();

void raiseError( error num );

char * errorMessage( error num );

// if "comp" is TRUE raise error "num" and return "ret"
#define raiseErrorIf( comp, num, ret ) \
			do{ if(!(comp)){raiseError(num); return ret;} }while(0)


#ifdef DEBUG
	#include <stdio.h>
	#undef raiseErrorIf
	#define raiseErrorIf( comp, num, ret )									\
		do{ if (!(comp)){													\
			raiseError(num);												\
			fprintf( stderr, "\nIn file %s\n%d :: %s => %s\nAsertion failed"\
			  ": %s\n", __FILE__,__LINE__,__func__,errorMessage(num),#comp);\
			return ret;														\
		  } }while(0)
#endif

#endif // ERROR_H