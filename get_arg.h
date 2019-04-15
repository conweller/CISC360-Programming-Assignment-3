/********************************************************
 * get_path.h                                           *
 *                                                      *
 * Author:  Connor Onweller                             *
 *                                                      *
 * Purpose: Contains methods to parse argv and argc     *
 *            from a string                             *
 ********************************************************/

#include <string.h>
#include <stdlib.h>

/*
 *  Function: get_argv
 *  ------------------
 *    Returns a list of arguments seperated by spaces from a given buffer
 *
 *    buf: the inputted string buffer to be parsed
 *    argc: the number of arguments in the buffer (necissary)
 *
 *    returns: the list of arguments contained within the input buffer
 */

char ** get_argv(char *buf, int argc);

/*
 *  Function: get_argc
 *  ------------------
 *    Returns the count of arguments contained in the buffer, i.e. the number of
 *      strings seperated by a space
 *
 *    buf: the inputted string buffer to be parsed
 *
 *    returns: the number of arguments in the inputted buffer
 */

int get_argc(char *buf);
