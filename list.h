#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/********************************************************
 * list.h                                               *
 *                                                      *
 * Author:  Connor, Onweller                            *
 *                                                      *
 * Purpose: Houses path_node struct for containing      *
 *            directory list, declares various          *
 *            directory listing functions               *
 ********************************************************/

struct list_node {
	char *element;
	struct list_node *next;
};

struct list_node * list_one_dir(int  argc, char ** argv);
void free_list(struct list_node * list);
/* void print_list(struct list_node * list); */
void print_list(struct list_node * list);

struct list_node ** list(int  argc, char ** argv);

void print_lists (int argc, char ** argv);
