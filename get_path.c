#include "get_path.h"
#include <string.h>

struct path_node *get_path() {
	char *p, *path;
	struct path_node *path_list, *tmp;

	p = getenv("PATH");


    if (p == NULL) {
        free(p);
        return NULL;

    }

	path = malloc((strlen(p)+1) * sizeof(char));
	strcpy(path, p);
	path[strlen(p)] = '\0';

	p = strtok(path, ":");
	path_list=NULL;
	do
	{
		if ( !path_list )
		{
			tmp = malloc(sizeof(struct path_node));
			path_list = tmp;
		}
		else
		{
			tmp->next = malloc(sizeof(struct path_node));
			tmp = tmp->next;
		}
		tmp->element = malloc(sizeof(char) * (strlen(p)+1));
		strcpy(tmp->element, p);
		/* printf("%s\n", path_list->element); */
		tmp->next = NULL;
	} while ( ( p = strtok(NULL, ":") ) );
	free(p);
	free(path);
	return path_list;
}


void free_path(struct path_node * path) {
	struct path_node *tmp;
	while(path != NULL){
		tmp = path;
		path = path->next;
		free(tmp->element);
		free(tmp);
	}
}
