/********************************************************
 * get_path.h                                           *
 *                                                      *
 * Author:  Connor, Onweller                            *
 *                                                      *
 * Purpose: Contains methods to parse argv and argc     *
 *            from a string                             *
 ********************************************************/

#include <string.h>
#include <stdlib.h>

char ** get_argv(char *a, int argc);
int get_argc(char *a);
