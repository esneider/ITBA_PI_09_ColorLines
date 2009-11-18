// error.h
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
	CORRUPTFILE
} error;

// Quita errores pasados
void clearError();

// Actual error code
error errorCode();

// if "comp" is TRUE raise error "num" and return "ret"
#define raiseErrorIf( comp, num, ret ) \
			do{ if(!(comp)){raiseError(num); return ret;} }while(0) 

// raise error "num"
void raiseError( error num );

// error message for error code "num"
char * errorMessage( error num );


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