// colors.c
#if defined(__unix__)

	#include "unix_colors.c"

#elif defined(__win32__)

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
