# rshell
### a simple command shell

Rshell executes commands read from standard input. Type any command, and Rshell will execute the corresponding program. Type "exit" to quit the shell.


Enter multiple commands on a single line by using these connectors:

&& executes the right side if the left side succeeds.

|| executes the right side if the left side fails.

; always executes the right side.

\# never executes the right side (thus making it a comment).


Connectors at the start of a line will proceed as if the left side succeeded, and connectors at the end of a line will be ignored. Connectors are evaluated left to right.

If multiple connectors are adjacent to each other, undefined behavior will result.


#### KNOWN ISSUES

Ctrl-D will attempt to run the current command before exiting, and may take two presses to work.


Arguments cannot have connectors, quotes, or other special characters in them.

## ls
### list files in a directory

ls lists information about one or more files or directories, using the current working directory if none are specified. By default, ls ignores files starting with .


#### ARGUMENTS

-a		Also list files starting with .

-l		List more detailed information about each file

-R		Recurse through all subdirectories.


### KNOWN ISSUES

ls currently prints files in a confusing manner.

