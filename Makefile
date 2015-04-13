VPATH = src
CFLAGS = -ansi -pedantic -Wall -Werror
#objects = $(addprefix obj/, main.o file.o)
objects = $(addprefix obj/, main.o)

bin/program: $(objects) | bin
	g++ -o $@ $(objects)

obj/%.o: %.cpp
	g++ $(CFLAGS) -c -o $@ $<
#obj/main.o: file.h
obj/main.o:

$(objects): | obj

bin:
	mkdir bin

obj:
	mkdir obj

clean:
	rm -rf obj bin

