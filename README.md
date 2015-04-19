# rshell
### a simple command shell

Rshell executes commands read from standard input.
Type any command, and Rshell will execute the corresponding program.
Type "exit" to quit the shell.

Enter multiple commands on a single line by using these connectors:
&& executes the right side if the left side succeeds.
|| executes the right side if the left side fails.
; always executes the right side.
# never executes the right side (thus making it a comment).

Connectors at the start of a line will proceed as if the left side succeeded.
Connectors at the end of a line will be ignored.
If multiple connectors are adjacent to each other, undefined behavior will result.
Connectors are evaluated left to right.

###KNOWN ISSUES
The shell will freak out on invalid commands, arguments, or connectors. (A memory leak would occur otherwise.)
Ctrl-D will exit the shell with an error.

