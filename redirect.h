/********************************************************
 * redirect.h                                           *
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

/* Redirect Overwrite */
extern const int REDIR_OW;
/* Redirect Standard Error */
extern const int REDIR_ER;
/* Redirect Append */
extern const int REDIR_AP;

/*
 *  Function: redirect
 *  ------------------
 *    Attempts to write the output of one command to a file.
 *
 *    cmd_argv: the arguments for the command
 *    cmd_argc: the number of arguments for the command
 *    filename: the file to be written to
 *    options: options given to the function (bitwise orred together):
 *        REDIR_OW: overwrite existing file, if the inputted filename exists
 *        REDIR_ER: write std_err to file as well as std_out
 *        REDIR_AP: append to the file
 */

void redirect(char ** cmd_argv, char* filename, int options);
