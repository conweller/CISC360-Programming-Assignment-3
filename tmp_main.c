#include "get_arg.h"
#include "get_path.h"
#include "watch.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
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

void handle_sigint(int sig) { // Catch SIGTERM and SIGTSTP
  sigignore(SIGTERM);
  sigignore(SIGTSTP);
}
void termsig(int sig) {} // Catch termsig as parent but not child

int main(void) {
  signal(SIGINT, termsig);
  signal(SIGTERM, handle_sigint);
  signal(SIGTSTP, handle_sigint);
  path = get_path();
  prompt = getcwd(NULL, 0);
  // Allocating the prompt prefix makes frees easier later on

  // We need to store the starting directory for cd - command
  printf("%s> ", prompt);

  //Boolean used by watchuser to tell it if it has been called yet by the shell
	int first_watchuser = 1;

  // Jump back here if we recieve a ^D
label:
  while (fgets(buf, MAXLINE, stdin) != NULL) {
    // Use buf to get argc and argv (called args and nargs respectively)
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
      if (!first_watchuser) {
		watchuser(args[0],0,0);
	  }
		free(prompt);
      free_path(path);
      for (int i = 0; i < nargs; i++) {
        free(args[i]);
      }
      free(args);
      exit(0);
    } else if (strcmp(args[0], "watchuser") == 0) {
		printf("Executing built-in command watchuser\n");
		if (nargs ==  1) {
			printf("Not enough arguments\n");
		} else if (nargs == 2) {
			printf("Searching for user\n");
			watchuser(args[1],0,first_watchuser);
			if (first_watchuser) {
				first_watchuser = 0;
			}
		} else if (nargs == 3) {
			if (strcmp(args[2], "off") == 0) {
				if (!first_watchuser) {
					watchuser(args[1],1,first_watchuser);
				}
			} else {
				printf("Invalid argument\n");
			}
		} else {
			printf("Too many arguments\n");
		}
  	} else if (strcmp(args[0], "watchmail") == 0) {
		printf("Executing built-in command watchmail\n");
		if (nargs ==  1) {
			printf("Not enough arguments\n");
		} else if (nargs == 2) {
			printf("Searching for file\n");
			watchmail(args[1],0);
		} else if (nargs == 3) {
			if (strcmp(args[2], "off") == 0) {
				watchmail(args[1],1);
			} else {
				printf("Invalid argument\n");
			}
		} else {
			printf("Too many arguments\n");
		}	
	} else if (strlen(args[0]) != 0) { // else try to execute external command
      if ((pid = fork()) < 0) {
        printf("fork error\n");
        exit(1);
      } else if (pid == 0) { /* child */
        execvp(args[0], args);
        printf("couldn't execute: %s\n", args[0]);
        exit(127);
      }
      if ((pid = waitpid(pid, &status, 0)) < 0)
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
