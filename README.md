# rshell
### a simple command shell

Rshell executes commands read from standard input.
It is not a complete shell (yet) as it does not allow the working directory to be changed.

Enter multiple commands on a single line by using these connectors:
&& executes the right side if the left side succeeds.
|| executes the right side if the left side fails.
; always executes the right side.
# never executes the right side (thus making it a comment).

If a line starts with a connector, the exit status from the previous line will be used.
If a line ends with a connector, the connector will be ignored.
If multiple connectors are adjacent to each other, undefined behaviour will result.
Connectors are evaulated left to right.


###KNOWN ISSUES
The shell does not actually execute commands yet.
Ctrl-D will create an infinite output loop.

