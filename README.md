## rshell
#### a simple command shell

Rshell executes commands read from standard input.
Type any command, and Rshell will execute the corresponding program.
Enter `Ctrl-C` to kill a command in progress.

Type `cd <PATH>` to change the working directory to <PATH>.
`cd -` will change to the previous working directory,
and `cd` will change to your home directory.

Type `exit` or enter `Ctrl-D` to quit the shell.


Enter multiple commands on a single line by using these connectors:

`&&` executes the right side if the left side succeeds.

`||` executes the right side if the left side fails.

`;` always executes the right side.

`#` never executes the right side (thus making it a comment).


Connectors at the start of a line will proceed as if the left side succeeded,
and connectors at the end of a line will be ignored.
Connectors are evaluated left to right.

If multiple connectors are adjacent to each other,
undefined behavior will result.


#### KNOWN ISSUES

`cd` will break if a bad path is entered. Restart the shell and try again.

If `Ctrl-C` is entered while the shell is idle, the user must press `ENTER` to clear the shell input.

If `Ctrl-D` is entered, Rshell will attempt to run the current command before exiting.
`Ctrl-D` may need to be pressed twice to properly exit.

Arguments cannot have connectors, quotes, or other special characters in them.

## ls
#### list files in a directory

ls lists information about one or more files or directories,
using the current working directory if none are specified.

If `-l` is not given, ls will arrange the files in rows and columns,
left-to-right then top-to-bottom.
ls lists files in non-case-sensitive alphabetical order, 
and normally ignores files starting with `.`


#### ARGUMENTS

`-a`

Also list files starting with `.`;
these files will be listed before all other files.

`-l`
				
List more detailed information about each file.

`-R`

List all subdirectories recursively.


### KNOWN ISSUES

Unlike GNU ls, this program will not print files in color.

