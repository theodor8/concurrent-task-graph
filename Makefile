

CC=gcc
CFLAGS=-pedantic -Wall


.PHONY: all clean run


all: bin/main

clean:
	$(RM) *~ src/*~ src/#* obj/*.o bin/*


obj/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

bin/main: obj/main.o obj/task.o
	$(CC) $^ -o $@


run: bin/main
	./bin/main

memrun: bin/main
	valgrind ./bin/main


