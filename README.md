# Programming Assignment 3
The main objective of this assignment is to adding more functionalities to your
shell from Project 2.

## TODO
* [x] background a 'job' using `&`
    * [ ] Extra Credit (10 points): Implement the `fg` built-in command
* [x] `watchuser`
    * [ ] Extra Credit (5 points): Also have your thread notify when the users
      being watched log off of a tty. Make it clear that you do this if you do,
      and document it well.
* [ ] `watchmail`
* [x] file redirections
* [ ] interprocess communication 

## Commenting Style Examples:
### File Headers:
```
/********************************************************
 * get_path.h                                           *
 *                                                      *
 * Author:  Connor, Onweller                            *
 *                                                      *
 * Purpose: Contains methods to parse argv and argc     *
 *            from a string                             *
 ********************************************************/
 ```
### Functions:
```
/*
 *  Function: get_argv
 *  ------------------
 *    Returns a list of arguments seperated by spaces from a given buffer
 *
 *    buf: the inputted string buffer to be parsed
 *    argc: the number of arguments in the buffer (necissary)
 *
 *    returns: the list of arguments contained within the input buffer
 */
```
### Structures:
```
/*
 *  Structure: list_node
 *  --------------------
 *    Stores an directory list as a linked list
 */
 ```
