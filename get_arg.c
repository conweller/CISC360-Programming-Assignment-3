/********************************************************
 * get_path.c                                           *
 *                                                      *
 * Author:  Connor, Onweller                            *
 *                                                      *
 * Purpose: Implements methods to parse argv and argc   *
 *            from a string                             *
 ********************************************************/

#include "get_arg.h"

/*
 *  Function: get_argv
 *  ------------------
 *    Returns a list of arguments from a given buffer, by parsing through it,
 *      and ignoring spaces, terminal characters, and newlines
 *
 *    buf: the inputted string buffer to be parsed
 *    argc: the number of arguments in the buffer (necissary)
 *
 *    returns: the list of arguments contained within the input buffer
 */

char **get_argv(char *buf, int argc) {
  char **argv = malloc((argc + 1) * sizeof(char *));
  char *cur_string = strtok(buf, " \n");
  int i = 0;
  while (cur_string != NULL && i < argc) {
    /* printf("yo\n"); */
    argv[i] = malloc(sizeof(char *) * (strlen(cur_string) + 1));
    strcpy(argv[i], cur_string);
    cur_string = strtok(NULL, " ");
    i++;
  }
  if (argv[argc - 1][strlen(argv[argc - 1]) - 1] == '\n')
    argv[argc - 1][strlen(argv[argc - 1]) - 1] = '\0';
  argv[argc] = NULL;
  return argv;
}

/*
 *  Function: get_argc
 *  ------------------
 *    Returns the count of arguments contained in the buffer, i.e. the number of
 *      strings seperated by a space
 *
 *    buf: the inputted string buffer to be parsed
 *
 *    returns: the number of arguments in the inputted buffer
 */

int get_argc(char *buf) {
  int argc = 0;
  if (strlen(buf) > 0) {
    if (buf[0] != ' ' && buf[0] != '\n')
      argc++;
    for (int i = 0; buf[i] != '\0' && buf[i]; i++)
      if (i != 0 && i + 1 != strlen(buf) - 1 && buf[i] == ' ' &&
          (buf[i + 1] != ' '))
        argc++;
  }
  return argc;
}
