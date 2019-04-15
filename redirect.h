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

const int REDIR_OW = 1;
const int REDIR_ER = 2;
const int REDIR_AP = 4;

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

void redirect(char ** cmd_argv, int cmd_argc, char* filename, int options);
