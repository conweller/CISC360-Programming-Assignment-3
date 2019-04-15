#include "get_arg.h"
#include "get_path.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#define _XOPEN_SOURCE 500
#define MAXLINE 128

extern char **environ;
char buf[MAXLINE];
char **args;
int nargs;
pid_t pid;
int status;
char *ptr;
char *prompt;
struct path_node *path;

int sigignore(int sig);
void handle_sigchld(int sig) {
  int saved_errno = errno;
  errno = saved_errno;
}
void handle_sigint(int sig) { // Catch SIGTERM and SIGTSTP
  sigignore(SIGTERM);
  sigignore(SIGTSTP);
}
void termsig(int sig) {} // Catch termsig as parent but not child
int bg_running = 0;
int main(void) {
  signal(SIGINT, termsig);
  signal(SIGTERM, handle_sigint);
  signal(SIGTSTP, handle_sigint);

  path = get_path();
  prompt = getcwd(NULL, 0);
  // Allocating the prompt prefix makes frees easier later on

  // We need to store the starting directory for cd - command
  printf("%s> ", prompt);
  // Jump back here if we recieve a ^D
label:
  while (fgets(buf, MAXLINE, stdin) != NULL) {
    while (waitpid((pid_t)(-1), 0, WNOHANG) > 0) {} //check if bg have terminated

    nargs = get_argc(buf);
    if (nargs == 0) {
      printf("%s> ", prompt);
      break;
    }

    args = get_argv(buf, nargs);

    if (strcmp(args[0], "pwd") == 0) { /* built-in command pwd */
      ptr = getcwd(NULL, 0);
      printf("CWD = [%s]\n", ptr);
      free(ptr);
    } else if (strcmp(args[0], "kill") == 0) { // kill inputted proccess
      if (nargs < 2)
        printf("Not enough arguments\n");
      else if (nargs == 2) {          // if just given proccess
        kill(atoi(args[1]), SIGTERM); // send SIGTERM
      } else if (nargs == 3) {        // if given signal
        char *sig = malloc(sizeof(char) * (strlen(args[1])));
        strcpy(sig, &args[1][1]); // Send signal to process
        kill(atoi(args[2]), atoi(sig));
        free(sig);
      }
    } else if (strcmp(args[0], "cd") == 0) {
      if (nargs == 1) {                           // cd home
        char *home = getenv("HOME");
        chdir(home);
      } else if (nargs > 2) {                     // cant do anything
        printf("Too many arguments\n");
      } else {                                    // normal cd
        chdir(args[1]);
      }
    } else if (strcmp(args[0], "pid") == 0) {
      printf("PID = %d\n", getpid());
    } else if (strcmp(args[0], "exit") == 0) { // Free everything and exit
      free(prompt);
      free_path(path);
      for (int i = 0; i < nargs; i++) {
        free(args[i]);
      }
      free(args);
      exit(0);
    } else if (strlen(args[0]) != 0) { // else try to execute external command
      char bg = 0;
      if (args[nargs-1][0] == '&'){ // if last arg is &

        bg = 1;             // mark as a background process
        free(args[nargs-1]); // reduce the numbber of arguments
        free(args[nargs]);
        args[nargs-1] = NULL;
        nargs --;
      }
      if ((pid = fork()) < 0) {
        printf("fork error\n");
        exit(1);
      } else if (pid == 0) { /* child */
        if (bg)         //if background process, make it's process group 0
          setpgid(0, 0); 
        execvp(args[0], args);
        printf("couldn't execute: %s\n", args[0]);
        exit(127);
      }
      if (!bg && (pid = waitpid(pid, &status, 0)) < 0) // only wait if fg
        printf("waitpid error\n");
    }
    for (int i = 0; i < nargs; i++) {
      free(args[i]);
    }
    free(args); // Reset prompt and arguments
    free(prompt);
    prompt = getcwd(NULL, 0); // Update prompt
    printf("%s> ", prompt);
  }
  clearerr(stdin); // Clear any EOF
  goto label;      // Reenter loop
  exit(0);
}
