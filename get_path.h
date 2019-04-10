#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

struct path_node *get_path();
void free_path(struct path_node * path);

struct path_node {
	char *element;
	struct path_node *next;
};

