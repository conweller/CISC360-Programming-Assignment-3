#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/********************************************************
 * watch.h                                              *
 *                                                      *
 * Author:  Freaney, Sophia                             *
 *                                                      *
 * Purpose: Contains methods for watchuser & watchmail  *
 *                                                      *
 ********************************************************/

/*
 *  Function: watchuser
 *  ------------------
 *    Returns a
 *
 *    user: username string
 *    off: int representing if 'off' is specified (1 for yes, 0 for no)
 *
 *    returns: the list of arg
 */
int watchuser(char *user, int off);


/*
 *  Function: threaduser
 *  ------------------
 *    Called by pthread in watchuser
 *
 *    user: username string
 *
 *    returns: the list of arg
 */
void *threaduser(void *something);

/*
 *  Function: watchmail
 *  ------------------
 *    Returns a list of
 *
 *    buf: the inputted string buffer to be parsed
 *    argc: the number of arguments in the buffer (necissary)
 *
 *    returns: the list of arg
 */
int watchmail(char *file);

/*
 *  Function: threadmail
 *  ------------------
 *    Called by pthread in watchuser
 *
 *    user: username string
 *
 *    returns: the list of arg
 */
void *threadmail(void *something);
