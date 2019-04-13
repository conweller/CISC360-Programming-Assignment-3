#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>

/********************************************************
 * list.h                                               *
 *                                                      *
 * Author:  Connor, Onweller                            *
 *                                                      *
 * Purpose: Houses path_node struct for containing      *
 *            directory list, declares various          *
 *            directory listing functions               *
 ********************************************************/

/*
 *  Function: list_one_dir
 *  ----------------------
 *    Lists the contents single directory (given by argv) in a linked list
 *
 *    argv: the list of command line arguments
 *    argc: the size of argv
 *
 *    returns: linked list representation of the contents of the directory
 *      argv[1] If the inputted directory does not exist, it's list is:
 *      <directory-name> --> <"No such directory">
 */

struct list_node *list_one_dir(int argc, char **argv);

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
 */

struct list_node ** list_dirs(int argc, char **argv);

/*
 *  Function: print_list
 *  --------------------
 *    Prints the values stored by a list_node struct in the format:
 *      <directory>:\n <file1>\n <file2>\n etc.
 *
 *    list: the list_node struct to be printed
 */

void print_list(struct list_node *list);

/*
 *  Function: print_lists
 *  ---------------------
 *    Prints the contents of each directory path found in argv in the format
 *      <directory1>:\n <file1>\n <file2>\n etc. <directory2>:\n <file1>\n etc.
 *
 *    list: the list_node struct to be printed
 */

void print_lists(int argc, char **argv);
void free_list(struct list_node *list);

/*
 *  Structure: list_node
 *  --------------------
 *    Stores an directory list as a linked list
 */

struct list_node {
  char *element;
  struct list_node *next;
};
