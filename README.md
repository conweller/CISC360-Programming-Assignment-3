# Programming Assignment 3
The main objective of this assignment is to adding more functionalities to your
shell from Project 2.

## Usage
Compile and run with:
```
make
```

## New Features
### Background with `&`
Executes a process in the background.  
Use by saying:
```
<external command> &
```
*Notes:*  
* The shell will not print a new prompt until the next command is entered (just
  like in `tcsh`)
* This will not work with other internal

### `watchuser` command
This command takes as its first argument a username to keep track of when the
user logs on

### `watchmail` command
This command takes as the first argument a name of a file, which must already
exist (give an error if it doesn't), to watch for new 'mails' in.

### File Redirection
Redirect the output of a command to a file with:
* `>`: redirect standard output of command on the left to file on the right
  (i.e. `<command> > file`)
* `>&`: redirect both standard output and standard error of command on the left
  to file on the right (i.e. `<command> >& file`).
* `>>`:  redirect standard output of command on the left to file on the right
  (i.e. `<command> >> file`), appending to the file.
* `>>&`: redirect both standard output and standard error of command on the left to
  file on the right (i.e. `<command> >>& file`), appending to the file.
* `<`: redirect standard input of command on left to come from file on the
  right (i.e. `<command> < file`).

The overwriting of files can be prevented with the `noclobber` command. When
called `noclobber` changes its variable value from 0 to 1 or 1 to 0. When this
value is 0 files can be overwritten with redirection commands. When the value
is 1, attempts to redirect to a file that exist will instead produce the
following output:
```
<filename>: File exists.
```

### Piping
Support for interprocess communications with:
* `|`: pipes the standard output of the command on the left to the standard
  input of the command on the right
* `|&`: pipes the standard output and standard error of the command on the left
  to the standard input of the command on the right
