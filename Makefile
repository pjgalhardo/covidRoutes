CC=gcc
CFLAGS=-pedantic -Wall -ansi 

all: proj2

proj1: proj2.c
	@${CC} ${CFLAGS} -o proj2 proj2.c

clean: 
	@rm proj2