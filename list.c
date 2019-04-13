#include "list.h"

/********************************************************
 * list.c                                               *
 *                                                      *
 * Author:  Connor, Onweller                            *
 *                                                      *
 * Purpose: Houses path_node struct for containing      *
 *            directory list, declares various          *
 *            directory listing functions               *
 ********************************************************/

/*
 *  Function: err_sys
 *  -------------------
 *    Prints the inputted error message x, and then exits with the status
 *      failure
 *
 *    x: the error message to print
 */

void err_sys(const char *x) {
  perror(x);
  exit(EXIT_FAILURE);
}

/*
 *  Function: list_one_dir
 *  ----------------------
 *    Lists the contents single directory (given by argv) in a linked list,
 *      obtained by finding that directory and parsing through its contents
 *
 *    argv: the list of command line arguments
 *    argc: the size of argv
 *
 *    returns: linked list representation of the contents of the directory
 *      argv[1] If the inputted directory does not exist, it's list is:
 *      <directory-name> --> <"No such directory">
 */

struct list_node *list_one_dir(int argc, char **argv) {
  DIR *dp;
  struct dirent *dirp;
  struct list_node *result = NULL;
  struct list_node *tmp;

  if (argc != 2)
    err_sys("need more arguments");

  if ((dp = opendir(argv[1])) == NULL) {
    result = malloc(sizeof(struct list_node));
    result->element = malloc(sizeof(char *) * (1 + strlen(argv[1])));
    strcpy(result->element, argv[1]);
    result->next = malloc(sizeof(struct list_node));
    result->next->element =
        malloc(sizeof(char *) * (1 + strlen("No such directory")));
    strcpy(result->next->element, "No such directory");
    result->next->next = NULL;
    return result;
  }

  result = malloc(sizeof(struct list_node));
  result->element = malloc(sizeof(char *) * (1 + strlen(argv[1])));
  strcpy(result->element, argv[1]);
  result->next = NULL;
  tmp = result;
  while ((dirp = readdir(dp)) != NULL) {
    tmp->next = malloc(sizeof(struct list_node));
    tmp = tmp->next;
    tmp->element = malloc(sizeof(char *) * (1 + strlen(dirp->d_name)));
    strcpy(tmp->element, dirp->d_name);
    tmp->next = NULL;
  }
  closedir(dp);
  return result;
}

/*
 *  Function: list_dirs
 *  -------------------
 *    Lists the contents of multiple directories given by argv
 *
 *    argv: the list of command line arguments
 *    argc: the size of argv
 *
 *    returns: linked list representation of the contents of all directories
 *      given by argv[1:]. If an inputted directory does not exist, it's list
 *      is:
 *      <directory-name> --> <"No such directory">
 *      if the size of argv is 1, list_dirs returns the contents of the current
 *      working directory as a linked list
 */

struct list_node **list_dirs(int argc, char **argv) {
  struct list_node **result = NULL;
  char **new_argv = NULL;
  if (argc == 1) {
    new_argv = malloc(sizeof(char *) * 3);
    new_argv[0] = "list";
    new_argv[1] = ".";
    new_argv[2] = NULL;
    result = malloc(sizeof(struct list_node *) * 2);
    result[0] = list_one_dir(2, new_argv);
    result[1] = NULL;
    free(new_argv);
  } else {
    result = malloc(sizeof(struct list_node *) * (argc + 1));
    for (int i = 1; i < argc; i++) {
      new_argv = malloc(sizeof(char *) * 3);
      new_argv[0] = "list";
      new_argv[1] = malloc(sizeof(char *) * (strlen(argv[i]) + 1));
      strcpy(new_argv[1], argv[i]);
      new_argv[2] = NULL;
      result[i - 1] = list_one_dir(2, new_argv);
      free(new_argv[1]);
      free(new_argv);
    }
    result[argc - 1] = NULL;
  }
  return result;
}

/*
 *  Function: print_list
 *  --------------------
 *    Prints the values stored by a list_node struct in the format:
 *      <directory>:\n <file1>\n <file2>\n etc.
 *
 *    list: the list_node struct to be printed
 */

void print_list(struct list_node *list) {
  struct list_node *tmp;
  tmp = list;
  printf("%s:\n", tmp->element);
  tmp = tmp->next;
  while (tmp != NULL) {
    printf("%s\n", tmp->element);
    tmp = tmp->next;
  }
  printf("\n");
}

/*
 *  Function: print_lists
 *  ---------------------
 *    Prints the contents of each directory path found in argv in the format
 *      <directory1>:\n <file1>\n <file2>\n etc. <directory2>:\n <file1>\n etc.
 *
 *    list: the list_node struct to be printed
 */

void print_lists(int argc, char **argv) {
  struct list_node **l = list_dirs(argc, argv);
  int i = 0;
  while (l[i] != NULL) {
    print_list(l[i]);
    free_list(l[i]);
    i++;
  }
  free(l);
}

/*
 *  Function: Free list
 *  --------------------
 *    Frees a list_node struct, by iterating through the list and free each
 *      node's contents and then the node itself
 *
 *    list: the list_node struct to be freed
 */

void free_list(struct list_node *list) {
  struct list_node *tmp;
  while (list != NULL) {
    tmp = list;
    list = list->next;
    free(tmp->element);
    free(tmp);
  }
}
