/**
* @file defines.h
* Contains several defines
*/

#ifndef DEFINES_H
#define DEFINES_H



/// maximum error length
#define MAX_ERR_LEN 1000
/// maximum command length
#define MAX_COM_LEN 100
/// maximum arguments to a command (counting the command)
#define MAX_ARGS 10


/// maximum main text lenght
#define MAX_TEXT 5000
/// maximum panel lines (a.k.a. commands history)
#define MAX_PANEL_LINES 25
/// actual number of panel lines @see drawPanel()
#define PANEL_LINES 10 // may be a cool variable


/// horizontal board lines enabled @see drawTable()
#define HOR_LINES true // may be a cool variable


/// colors enablead @see colors.c
#define USE_COLORS true // may be a cool variable


/// input options @see menu.c
#define MAX_MINUTES 10000 // Max playing time: 1 week: 7 * 24 * 60 = 10080
#define MIN_TAB_DIM 5
#define MAX_TAB_DIM 150 // I don't think you have a bigger monitor
#define MIN_COLORS 2
#define MAX_COLORS 9
#define MIN_TOK_PER_LINE 3


/// edit distance options @see editDistance()
#define MIN_SIMILARITY 0.65
#define MIN_EDIT_LEN 3
#define MAX_EDIT_LEN 35


/// maximum processing time for {@link randFill()}
#define MAX_WAITING_TIME 15



#endif // DEFINES_H
