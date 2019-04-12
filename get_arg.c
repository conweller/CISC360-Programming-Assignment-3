/********************************************************
 * get_path.c                                           *
 *                                                      *
 * Author:  Connor, Onweller                            *
 *                                                      *
 * Purpose: Implements methods to parse argv and argc   *
 *            from a string                             *
 ********************************************************/

#include "get_arg.h"

// Get the argv from user input
char ** get_argv(char *a, int argc) {        // Get the inputted arguments
    // Malloc extra spot for null array
    char ** argv = malloc((argc + 1) * sizeof(char *));
    char *cur_string = strtok(a, " \n");
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

int get_argc(char *a) {        // Get the number of arguments
    int argc = 0;
    if (strlen(a) > 0) {
        if (a[0] != ' ' && a[0] != '\n')
            argc++;
        for (int i = 0; a[i] != '\0' && a[i]; i++)
            if (i != 0 && i + 1 != strlen(a) - 1 && a[i] == ' ' && (a[i + 1] != ' '))
                argc++;
    }
    return argc;
}
