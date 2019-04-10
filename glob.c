#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char ** glob_string(char * command, char * input, int *size) {
	glob_t paths;
	int csource;
	char **p;
	csource = glob(input, 0, NULL, &paths);
	int count = 0;

	if (csource == 0) {
		p = malloc(sizeof(char **) * (paths.gl_pathc + 2));
		p[0] = malloc(sizeof(char*) *(1+strlen(command)));
		strcpy(p[0], command);
		for (int i = 1; i < paths.gl_pathc; i++) {
			p[i] = malloc(sizeof(char *)*(1+strlen(paths.gl_pathv[i])));
			strcpy(p[i], paths.gl_pathv[i]);
			count ++;
		}
		p[paths.gl_pathc] = NULL;
		*size = count+1;
		globfree(&paths);
		return p;
	}
	return NULL;
}

/* // Globs args 1 through n (not arg[0]) */
/* char ** glob_argv(char ** argv, int * argc) { */
/* 	char *** globs = malloc(sizeof(char * *) * (*argc + 1)); */
/* 	char ** new_argv; */
/* 	char * tmp; */
/* 	int j; */
/* 	int i = 1; */
/* 	int count = 0; */
/* 	while (argv[i] != NULL) { */
/* 		tmp = argv[i]; */
/* 		if (strstr(tmp, "*") != NULL || strstr(tmp, "*") != NULL) { */
/* 			int  size = 0; */
/* 			globs[i] = glob_string(tmp, &size); */
/* 			count += size; */
/* 		} */
/* 		else { */
/* 			globs[i] = malloc(sizeof(char **) *2); */
/* 			globs[i][0] = &tmp[i]; */
/* 			globs[i][1] = NULL; */
/* 			count ++; */
/* 		} */
/* 		i++; */
/* 	} */
/* 	globs[*argc] = NULL; */
/* 	new_argv = malloc(sizeof(char*) * (count+1)); */
/* 	char ** cur_glob; */
/* 	new_argv[0] = argv[0]; */
/* 	i = 1; */
/* 	int index = 1; */
/* 	while(globs[i] != NULL){ */
/* 		cur_glob = globs[i]; */
/* 		j = 0; */
/* 		while(cur_glob[j] !=NULL){ */
/* 			new_argv[index] = cur_glob[j]; */
/* 			j++; */
/* 			index++; */
/* 		} */
/* 		i ++; */
/* 	} */
/* 	for (int k = 0; k < count; k++) { */
/* 		/1* printf("%s\n",new_argv[k]); *1/ */
/* 	} */
/* 	return new_argv; */
/* 	*argc = count; */

/* } */
