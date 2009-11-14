// defines.h
#ifndef DEFINES_H
#define DEFINES_H


#define MAX_ERR_LEN 200
#define MAX_ARGS 10
#define MAX_COM_LEN 100

#define MAX_PANEL_LINES 10
#define MAX_TEXT 5000

#define HOR_LINES true
#define VER_LINES true

#define USE_COLORS true

#define MIN_MINUTES 1
#define MAX_MINUTES 10000 // Max playing time: 1 week: 7 * 24 * 60 = 10080
#define MIN_TAB_DIM 5
#define MAX_TAB_DIM 150 // I don't think you have a bigger monitor
#define MIN_COLORS 2
#define MAX_COLORS 9
#define MIN_TOK_PER_LINE 3
#define MAX_TOK_PER_LINE 700 // 500 * sqrt(2) = 707


#endif // DEFINES_H