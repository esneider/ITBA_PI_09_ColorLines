// colors.c
#if defined(__unix__)

	#include "unix_colors.c"

#elif defined(__win32__)   || defined(__WIN32__) || \
	  defined(win32)       || defined(WIN32)     || \
	  defined(__win32)     || defined(__WIN32)   || \
	  defined(__windows__) || defined(__WINDOWS__)

	#include "win_colors.c"

#else

	#include "no_colors.c"

#endif
/*
#include "colors.h"

inline void textcolor( color c ){
	textcolor_(c);
}

inline void backcolor( color c ){
	backcolor_(c);
}

inline void textattr( attr a ){
	textattr_(a);
}
*/
