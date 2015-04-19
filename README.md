# rshell
### a simple command shell

Rshell executes commands read from standard input.
It is not a complete shell (yet) as it does not allow the working directory to be changed.

Type any command, and Rshell will execute the corresponding program in $PATH.
Type "exit" to quit the shell.

#### This stuff does not actually work yet!
Enter multiple commands on a single line by using these connectors:
&& executes the right side if the left side succeeds.
|| executes the right side if the left side fails.
; always executes the right side.
# never executes the right side (thus making it a comment).

If a line starts with a connector, the exit status from the previous line will be used.
If a line ends with a connector, the connector will be ignored.
If multiple connectors are adjacent to each other, undefined behavior will result.
Connectors are evaluated left to right.


###KNOWN ISSUES
The shell currently does not accept connectors or comments.
The shell may freak out when a command is entered and not execute it correctly.
Ctrl-D will create an infinite output loop.

