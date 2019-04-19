
#include "my_pipe.h"

/*
 *  Function: change_output
 *  -----------------------
 *    Changes std_out, std_err, or std_in to a file
 *
 *    dest: The fid of that to_close is being changed to
 *    to_close: The fid to close (e.g. std_out, std_err, std_in)
 *    oflags: open flags being passed to open (see open(2) for more details)
 */

void change_pipe(int dest, int to_close) {
  if (dest < 0 )
    perror("open error");
  close(to_close);
  dup(dest);
  close(dest);
}

void open_pipe(char ** right_argv, char ** left_argv, char err) {
  int pipefd[2];
  pipe(pipefd);
  int pid;
  if ((pid = fork()) < 0) {
    perror("fork error(1)");
    exit(0);
  }  else if (pid > 0) {                         /* parent */
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
      /* close(0); */
      close(pipefd[1]);
      change_pipe(pipefd[0], 0);
      execvp(right_argv[0], right_argv);
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
    execvp(left_argv[0], left_argv);
    exit(0);
  }
}
