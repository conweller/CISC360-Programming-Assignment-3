/********************************************************
 * my_pipe.h                                           *
 *                                                      *
 * Author:  Connor Onweller                             *
 *                                                      *
 * Purpose: Contains methods to redirect output form    *
 * one command to a file                                *
 ********************************************************/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 *  Function: open_pipe
 *  --------------
 *    Sets output of one command as the input of another
 *
 *    right: The command to the right of the pipe symbol
 *    left: The command to the left of the pipe symnbol
 *    err: if true, it will pipe std_err to right command
 */

void open_pipe(char ** right_argv, char ** left_argv, char err);
