#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"


void err_sys(const char* x) {
	perror(x);
	exit(1);
}
struct list_node * list_one_dir(char ** argv, int  argc) {
	DIR				*dp;
	struct dirent	*dirp;
	struct list_node * result =NULL;
	struct list_node * tmp;

	if (argc != 2)
		err_sys("need more arguments");

	if ((dp = opendir(argv[1])) == NULL){
		/* printf("no such file or directory\n"); */
		result=malloc(sizeof(struct list_node));
		result->element = malloc(sizeof(char *) * (1+strlen(argv[1])));
		strcpy(result->element, argv[1]);
		result->next = malloc(sizeof(struct list_node));
		result->next->element = malloc(sizeof(char *) * (1+strlen("No such directory")));
		strcpy(result->next->element, "No such directory");
		result->next->next = NULL;
		return result;
	}
		/* err_sys("opendir error"); */

	result=malloc(sizeof(struct list_node));
	result->element = malloc(sizeof(char *) * (1+strlen(argv[1])));
	strcpy(result->element, argv[1]);
	result->next = NULL;
	tmp = result;
	while ((dirp = readdir(dp)) != NULL){
		/* else { */
		tmp->next = malloc(sizeof(struct list_node));
		tmp = tmp->next;
		tmp->element = malloc(sizeof(char *) * (1+strlen(dirp->d_name)));
		strcpy(tmp->element, dirp->d_name);
		tmp->next = NULL;
		/* } */
	}
	closedir(dp);
	return result;
}
struct list_node ** list(char ** argv, int  argc) {
	struct list_node ** result = NULL;
	char ** new_argv= NULL;
	if (argc == 1){
		new_argv = malloc(sizeof(char *) * 3);
		new_argv[0] = "list";
		new_argv[1] = ".";
		new_argv[2] = NULL;
		result = malloc(sizeof (struct list_node*) * 2);
		result[0] = list_one_dir(new_argv, 2);
		result[1] = NULL;
		free(new_argv);
	} 
	else {
		result = malloc(sizeof (struct list_node*) * (argc+1));
		for (int i=1; i<argc; i++){
			new_argv = malloc(sizeof(char *) * 3);
			new_argv[0] = "list";
			new_argv[1] = malloc(sizeof(char *) * (strlen(argv[i]) + 1));
			strcpy(new_argv[1], argv[i]);
			new_argv[2] = NULL;
			/* list_one_dir(new_argv, 2); */
			result[i-1] = list_one_dir(new_argv, 2);
			/* if ((asprintf(&result, "%s%s", result, list_one_dir(new_argv, 2)))==-1) */
			/* 	err_sys("asprintf error"); */
			free(new_argv[1]);
			free(new_argv);
		}
		result[argc-1] = NULL;
	}
	return result;
}

void free_list(struct list_node * list) {
	struct list_node *tmp;
	while(list != NULL){
		tmp = list;
		list = list->next;
		free(tmp->element);
		free(tmp);
	}
}

void print_list(struct list_node * list) {
	struct list_node *tmp;
	tmp = list;
	printf("%s:\n", tmp->element);
	tmp = tmp->next;
	while(tmp != NULL){
		printf("%s\n", tmp->element);
		tmp = tmp->next;
	}
	printf("\n");
}
