#include "get_path.h"
#include "get_arg.h"
#include "list.h"
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

char *which(char **args, int nargs, struct path_node *env_path);
void where(char **args, int nargs, struct path_node *env_path);


void handle_sigint(int sig) {   // Catch SIGTERM and SIGTSTP
    sigignore(SIGTERM);
    sigignore(SIGTSTP);
}
void termsig(int sig) {}        // Catch termsig as parent but not child

int main(void) {
    signal(SIGINT, termsig);
    signal(SIGTERM, handle_sigint);
    signal(SIGTSTP, handle_sigint);
    path = get_path();
    prompt = getcwd(NULL, 0);
    // Allocating the prompt prefix makes frees easier later on
    char *prompt_pref = malloc(sizeof(char));
    strcpy(prompt_pref, "");

    // We need to store the starting directory for cd - command
    char *prev_dir = getcwd(NULL, 0);
    printf("%s%s> ", prompt_pref, prompt);

    // Jump back here if we recieve a ^D
label:
    while (fgets(buf, MAXLINE, stdin) != NULL) {
        // Use buf to get argc and argv (called args and nargs respectively)
        nargs = get_argc(buf);
        if (nargs == 0) {
            printf("%s%s> ", prompt_pref, prompt);
            break;
        }

        args = get_argv(buf, nargs);

        if (strcmp(args[0], "pwd") == 0) { /* built-in command pwd */
            ptr = getcwd(NULL, 0);
            printf("CWD = [%s]\n", ptr);
            free(ptr);
        } else if (strcmp(args[0], "prompt") == 0) { /* built-in command pwd */
            // TODO add prefix, need to free prompt_pref

            if (nargs == 1) {
                printf("enter new prompt prefix\n");
                free(prompt_pref);
                prompt_pref = malloc(sizeof(char*));
                fgets(prompt_pref, MAXLINE, stdin);
                int i = 0;
                while(prompt_pref[i] != '\0'){
                    if (prompt_pref[i] == '\n') {
                        prompt_pref[i] = '\0';
                    }
                    i++;
                }
                
            }
            if (nargs == 2) {
                free(prompt_pref);
                prompt_pref = malloc(sizeof(char*) * (strlen(args[1])) +1);
                strcpy(prompt_pref, args[1]);
                strcat(prompt_pref, " ");
            }
        } else if (strcmp(args[0], "kill") == 0) {      // kill inputted proccess
            if (nargs < 2)
                printf("Not enough arguments\n");
            else if (nargs == 2) {                      // if just given proccess
                kill(atoi(args[1]), SIGTERM);           // send SIGTERM
            } else if (nargs == 3) {                    // if given signal
                char *sig = malloc(sizeof(char) * (strlen(args[1])));
                strcpy(sig, &args[1][1]);               // Send signal to process
                kill(atoi(args[2]), atoi(sig));
                free(sig);
            }
        } else if (strcmp(args[0], "list") == 0) {      // built in list function
          print_lists(nargs, args);
        } else if (strcmp(args[0], "cd") == 0) {
            if (nargs == 1) {                           // cd home
                free(prev_dir);
                prev_dir = getcwd(NULL, 0);
                char *home = getenv("HOME");
                chdir(home);
            } else if (nargs > 2) {                     // cant do anything
                printf("Too many arguments\n");
            } else if (strcmp(args[1], "-") == 0) {     // cd to prev_dir
                char *tmp = getcwd(NULL, 0);
                chdir(prev_dir);
                free(prev_dir);
                prev_dir = tmp;
            } else {                                    // normal cd
                free(prev_dir);
                prev_dir = getcwd(NULL, 0);
                chdir(args[1]);
            }
        } else if (strcmp(args[0], "printenv")==0){     // built in printenv
            if (nargs == 1){
                char *s = *environ;
                for (int i = 1; s; i++) {
                    printf("%s\n", s);
                    s = *(environ+i);
                }
            } else if (nargs == 2) {
                char * env_var = getenv(args[1]);
                printf("%s\n", env_var);
            }
        } else if (strcmp(args[0], "setenv") == 0){
            if (nargs == 1) {
                char *s = *environ;
                for (int i = 1; s; i++) {
                    printf("%s\n", s);
                    s = *(environ+i);
                }
            }
            else if (nargs == 2) {
                unsetenv(args[1]);
            }
            else if (nargs == 3) {
                setenv(args[1], args[2], 1);
            }
            if (nargs > 3) {
                printf("too many arguments\n");
            } else if (nargs > 1 && strcmp(args[1],"PATH") == 0) {
                free_path(path);
                path = get_path();
            }
        } else if (strcmp(args[0], "pid") == 0) {
            printf("PID = %d\n", getpid());
        } else if (strcmp(args[0], "which") == 0) { // call which from which.c
            char **cur_args = malloc(sizeof(char *) * 3);
            char *tmp;
            for (int i = 1; i < nargs; i++) {       // for each item in args call
                cur_args[0] = "which";              // which
                cur_args[1] = malloc(sizeof(char *) * (strlen(args[i]) + 1));
                cur_args[1] = strcpy(cur_args[1], args[i]);
                cur_args[2] = NULL;
                tmp = which(cur_args, 2, path);
                printf("%s\n", tmp);
                free(tmp);
                free(cur_args[1]);
            }
            free(cur_args);
        } else if (strcmp(args[0], "where") == 0) { // Call where from which.c
            char **cur_args = malloc(sizeof(char *) * 3);
            for (int i = 1; i < nargs; i++) {       // for each item in args call
                cur_args[0] = "where";              // which
                cur_args[1] = malloc(sizeof(char *) * (strlen(args[i]) + 1));
                cur_args[1] = strcpy(cur_args[1], args[i]);
                cur_args[2] = NULL;
                where(cur_args, 2, path);
                fflush(stdout);
                free(cur_args[1]);
            }
            free(cur_args);

        } else if (strcmp(args[0], "exit") == 0) {  // Free everything and exit
            free(prompt);
            free_path(path);
            for (int i = 0; i < nargs; i++) {
                free(args[i]);
            }
            free(args);
            free(prev_dir);
            free(prompt_pref);
            exit(0);
        }

        else if (strlen(args[0]) != 0) { // else try to execute external command
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

        free(args);                     // Reset prompt and arguments
        free(prompt);
        prompt = getcwd(NULL, 0);       // Update prompt
        printf("%s%s> ", prompt_pref, prompt);
    }
    clearerr(stdin);                    // Clear any EOF
    goto label;                         // Reenter loop
    exit(0);
}
