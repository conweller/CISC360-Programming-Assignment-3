#include "get_path.h"
#include<stdio.h>
#include <assert.h>
#include"list.h"
#include<string.h>


void where(char **args, int nargs, struct path_node * env_path){
	assert(nargs == 2);
	
	struct path_node *path = env_path;
	while(path != NULL) {
		char ** cur_args = malloc(sizeof(char*)*3);
		cur_args[0]="list";
		cur_args[1]=malloc(sizeof(char*) * (strlen(path->element)+1));
		strcpy(cur_args[1], path->element);
		cur_args[2]=NULL;
		struct list_node * current_dir_head = list_one_dir(2, cur_args);
		struct list_node * current_dir =  current_dir_head;
		if (current_dir != NULL){
			char * cur_dir_name = current_dir->element;
			current_dir = current_dir->next;
			while (current_dir){
				if (strcmp(current_dir->element, args[1]) == 0){
					printf("%s/%s\n", cur_dir_name,  args[1]);
					break;
				}
				current_dir = current_dir->next;
			}
		}
		free_list(current_dir_head);
		free(cur_args[1]);
		free(cur_args);
		/* struct path_node * tmp = path; */
		path = path->next;
		/* free(tmp); */
	}
	/* free_path(path_head); */

}

char * which(char **args, int nargs, struct path_node * env_path){
	assert(nargs == 2);
	
	struct path_node *path = env_path;
	while(path != NULL) {
		char ** cur_args = malloc(sizeof(char*)*3);
		cur_args[0]="list";
		cur_args[1]=malloc(sizeof(char*) * (strlen(path->element)+1));
		strcpy(cur_args[1], path->element);
		cur_args[2]=NULL;
		struct list_node * current_dir_head = list_one_dir(2,cur_args);
		struct list_node * current_dir =  current_dir_head;
		if (current_dir != NULL){
			char * cur_dir_name = current_dir->element;
			current_dir = current_dir->next;
			while (current_dir){
				if (strcmp(current_dir->element, args[1]) == 0){
					char * result = malloc(sizeof(char) * (strlen(cur_dir_name) +  (strlen(args[1]) + 2)));
					strcpy(result, cur_dir_name);
					strcat(result, "/");
					strcat(result, args[1]);
					free_list(current_dir_head);
					free(cur_args[1]);
					free(cur_args);
					return result;

				}
				current_dir = current_dir->next;
			}
		}
		free_list(current_dir_head);
		free(cur_args[1]);
		free(cur_args);
		/* struct path_node * tmp = path; */
		path = path->next;
		/* free(tmp); */
	}
	char * not_found = malloc(sizeof(char*));
	strcpy(not_found, "");
	return not_found;
	/* free_path(path_head); */

}


