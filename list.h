#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


struct list_node {
	char *element;
	struct list_node *next;
};

struct list_node * list_one_dir(char ** argv, int  argc);
void free_list(struct list_node * list);
/* void print_list(struct list_node * list); */
void print_list(struct list_node * list);

struct list_node ** list(char ** argv, int  argc);
