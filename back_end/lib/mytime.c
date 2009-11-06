// mytime.c
#include "error.h"
#include "mytime.h"

time_t start_time;

void reset_time(){
	start_time = time(NULL);
	raise_error_if( start_time != -1, TIMEERROR, );
}

int time_left( game * G ){
	return G->un.time - time(NULL) + start_time;
}

