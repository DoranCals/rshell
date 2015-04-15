# rshell
### a simple command shell

Rshell executes commands read from standard input.
It is not a complete shell (yet) as it does not allow the working directory to be changed.

Enter multiple commands on a single line by using these connectors:
&& executes the right side if the left side succeeds.
|| executes the right side if the left side fails.
; always executes the right side.
# never executes the right side (thus making it a comment).

Any connector without a command on each side of it will result in a syntax error.
Connectors are evaulated left to right.


###KNOWN ISSUES
The shell does not actually execute commands yet.
Ctrl-D will create an infinite output loop.

