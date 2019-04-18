/********************************************************
 * get_path.h                                           *
 *                                                      *
 * Author:  Connor Onweller                             *
 *                                                      *
 * Purpose: Houses path_node struct for containing      *
 *            environment paths, declares various       *
 *            path_node functions                       *
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 *  Function: get_path
 *  ------------------
 *    Returns the environment path as a linked list
 *
 *    returns: the environment path as a path_node struct
 *    (linked list)
 */

struct path_node *get_path();

/*
 *  Function: free_path
 *  -------------------
 *    Frees a path_node struct
 *
 *    path: the path_node struct to be freed
 */

void free_path(struct path_node *path);

/*
 *  Structure: path_node
 *  --------------------
 *    Stores an environment path as a linked list
 */

struct path_node {
  char *element;
  struct path_node *next;
};
