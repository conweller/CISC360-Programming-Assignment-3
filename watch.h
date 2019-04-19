#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <utmpx.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

/********************************************************
 * watch.h                                              *
 *                                                      *
 * Author:  Freaney, Sophia                             *
 *                                                      *
 * Purpose: Contains methods for watchuser & watchmail  *
 *                                                      *
 ********************************************************/

struct Node {
  char *name;
  int on;
  char *tty;
  char *host;
  pthread_t id;
  struct Node *next;
};

/*
 *  Function: insert
 *  ------------------
 *    Inserts a new username into the linked list of watched names
 *
 *    name: username string
 *
 *    return: void
 */
void insert(char *name, pthread_t id, char *type);

/*
 *  Function: delete
 *  ------------------
 *    Deletes a username from the linked list of watched names
 *
 *    name: username string
 */
void delete(char *name, char *type);

/*
 *  Function: freeList
 *  ------------------
 *    Free the linked list of watched names when exit is called
 *
 *    returns: void
 */
void freeList(char *type);

/*
 *  Function: watchuser
 *  ------------------
 *    Called by main to monitor usernames that log on and off the system
 *
 *    user: username string
 *    off: int representing if 'off' is specified (1 for yes, 0 for no)
 *    first: int representing if watchuser has been called yet (1 for yes, 0 for no)
 *
 *    returns: the list of arg
 */
int watchuser(char *user, int off, int first);

/*
 *  Function: threaduser
 *  ------------------
 *    Called by pthread in watchuser; runs while loop to check for watched user activity
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
int watchmail(char *file, int off);

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
