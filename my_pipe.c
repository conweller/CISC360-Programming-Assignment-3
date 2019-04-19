/********************************************************
 * my_pipe.c                                            *
 *                                                      *
 * Author:  Connor Onweller                             *
 *                                                      *
 * Purpose: Contains methods to pipe output from one    *
 * command to the input of another                      *
 ********************************************************/

#include "my_pipe.h"


/*
 *  Function: open_pipe
 *  --------------
 *    Sets output of one command as the input of another,
 *      Forks once sets childs output (or output & error) to 
 *      go to pipe. Then waits for child to complete, and forks
 *      again from parent, this time setting the new child's input
 *      as the pipe, executing with that input
 *
 *    right: The command to the right of the pipe symbol
 *    left: The command to the left of the pipe symnbol
 *    err: if true, it will pipe std_err to right command
 */

void open_pipe(char ** right_argv, char ** left_argv, char err) {
  int pipefd[2];
  pipe(pipefd);
  int pid;
  if ((pid = fork()) < 0) {
    perror("fork error(1)");
    exit(0);
  }  else if (pid > 0) {
    if (waitpid(pid, NULL, 0) < 0) {
      perror("waitpid error");
      exit(1);
    }
    if ((pid = fork()) < 0) {
      perror("fork error(2)");
      exit(0);
    } else if (pid > 0) {
      if (waitpid(pid, NULL, 0) < 0) {
        perror("waitpid error");
        exit(1);
      }
      return;
    } else if (pid == 0) {
      close(pipefd[1]);
      close(0);
      dup2(pipefd[0], 0);  // send stdout to the pipe
      if (execvp(right_argv[0], right_argv) < 0)
        execv(right_argv[0], right_argv);
      exit(0);
    }
  } else if (pid == 0) {
    close(1);
    if (err)
      close(2);
    close(pipefd[0]);
    dup2(pipefd[1], 1);  // send stdout to the pipe
    if (err)
      dup2(pipefd[1], 2);  // send stderr to the pipe
    if (execvp(left_argv[0], left_argv) < 0)
      execv(left_argv[0], left_argv);
    exit(0);
  }
}
