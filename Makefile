VPATH = src
FLAGS = -ansi -pedantic -Wall -Werror -std=c++11

all: rshell ls rm

rshell:
	mkdir -p bin
	g++ $(FLAGS) -o bin/rshell src/main.cpp

ls:
	mkdir -p bin
	g++ $(FLAGS) -o bin/ls src/ls.cpp

rm:
	mkdir -p bin
	g++ $(FLAGS) -o bin/rm src/rm.cpp

clean:
	rm -rf bin

