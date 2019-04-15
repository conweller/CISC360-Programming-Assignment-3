/********************************************************
 * redirect.c                                           *
 *                                                      *
 * Author:  Connor Onweller                             *
 *                                                      *
 * Purpose: Contains methods to redirect output form    *
 * one command to a file                                *
 ********************************************************/

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

void redirect(char ** cmd_argv, int cmd_argc, char* filename, int options) {

}
