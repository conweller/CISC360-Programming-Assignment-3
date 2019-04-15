/********************************************************
 * redirect.c                                           *
 *                                                      *
 * Author:  Connor Onweller                             *
 *                                                      *
 * Purpose: Contains methods to redirect output form    *
 * one command to a file                                *
 ********************************************************/

#include "redirect.h"

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
  int fd[2];
  pid_t pid;

  if (pipe(fd) < 0) {
    perror("pipe error");
    exit(1);
  } 

  // Open file
  
  if ((pid = fork()) < 0) {
    perror("fork error");
    exit(0);
  }



  if (options & REDIR_ER) {
    // Error
  } else {
    // No error
  }
  if (options & REDIR_OW) {
    // Overwrite
  } else {
    // No Overwrite
  }
  if (options & REDIR_AP) {
    // Append
  } else {
    // No Append
  }

}
