CC=clang
CFLAGS=-Wall -Wextra -Werror -std=c11 -pedantic -Wno-unused-parameter -gdwarf-4

all: example1 example2

example1: example1.o dining.o utils.o
example2: example2.o dining.o utils.o

%.o : %.c
	$(CC) $(CFLAGS) $< -c

.PHONY: clean
clean:
	- rm -f *.o example1 example2
