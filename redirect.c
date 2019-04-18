/********************************************************
 * redirect.c                                           *
 *                                                      *
 * Author:  Connor Onweller                             *
 *                                                      *
 * Purpose: Contains methods to redirect output form    *
 * one command to a file                                *
 ********************************************************/

#include "redirect.h"

int const REDIR_OW = 1;
int const REDIR_ER = 2;
int const REDIR_AP = 4;


/*
 *  Function: change_output
 *  -----------------------
 *    Changes std_out, std_err, or std_in to a file
 *
 *    dest: The filename of that to_close is being changed to
 *    to_close: The fid to close (e.g. std_out, std_err, std_in)
 *    oflags: open flags being passed to open (see open(2) for more details)
 */

void change_output(char *dest, int to_close, int oflags) {
  int fid = open(dest, oflags, 0666);
  if (fid < 0 )
    perror("open error");
  close(to_close);
  dup(fid);
  close(fid);
}

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

void redirect(char **cmd_argv,  char *filename, int options) {
  pid_t pid;
  int file_options = O_CREAT | O_RDWR;

  if (options & REDIR_AP) 
    file_options |= O_APPEND;

  if (!(options & REDIR_OW) && !access(filename, F_OK)) {
    char ans, c;
    printf("%s: File exits.\n", filename);
    return;
  }
  if (!access(filename, F_OK) && !(options & REDIR_AP)){
    file_options |= O_TRUNC;
  }

  if ((pid = fork()) < 0) {
    perror("fork error");
    exit(0);
  } else if (pid > 0) {
    if (waitpid(pid, NULL, 0) < 0) {
      perror("waitpid error");
      exit(1);
    }
  } else if (pid == 0) {
    change_output(filename, 1, file_options|O_APPEND);
    if (options & REDIR_ER) 
      change_output(filename, 2, file_options);
    if (execvp(cmd_argv[0], cmd_argv) == -1){
      perror("exec error");
    }
    exit(0);
  }
  if (options & REDIR_ER) 
    change_output("/dev/tty", 2, O_WRONLY);
  change_output("/dev/tty", 1, O_WRONLY);
}

