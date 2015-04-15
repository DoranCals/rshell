VPATH = src
FLAGS = -ansi -pedantic -Wall -Werror

all: rshell

rshell:
	rm -rf bin
	mkdir bin
	g++ $(FLAGS) -std=c++11 -o bin/rshell src/main.cpp

clean:
	rm -rf bin

