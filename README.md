# rshell
### a simple command shell

Rshell executes commands read from standard input.
It is not a complete shell (yet) as it does not allow the working directory to be changed.

Enter multiple commands on a single line by using these connectors:
`&& || ; #`
&& will execute the right command only if the left command succeeded, while || will execute the right command only if the left command failed.
; will always execute the right command, while # will never execute the right command (thus making it a comment.)
Connectors are evaulated left to right, and any connectors at the beginning or end of a line are ignored.

###KNOWN ISSUES
Ctrl-D will create an infinite output loop.

