# rshell
### a simple command shell

Rshell executes commands read from standard input.
It is not a complete shell (yet) as it doesn't allow the working directory to be changed.

If a connector or an invalid command is found at the beginning of a line, the "first" command will be interpreted as having failed.

Here's some code: `print("Hello World!")`
and here's some more code:
```
if true then
	print("Hello World!")
end
```
