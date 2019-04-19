/********************************************************
 * my_pipe.h                                            *
 *                                                      *
 * Author:  Connor Onweller                             *
 *                                                      *
 * Purpose: Contains methods to pipe output from one    *
 * command to the input of another                      *
 ********************************************************/
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 *  Function: open_pipe
 *  --------------
 *    Sets output of one command as the input of another
 *
 *    right: The command to the right of the pipe symbol
 *    left: The command to the left of the pipe symnbol
 *    err: if true, it will pipe std_err to right command
 */

void open_pipe(char **right_argv, char **left_argv, char err);
