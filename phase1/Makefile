CC = gcc
CFLAGS = -Og
LDLIBS = -lpthread

PROGS = myshell

all: $(PROGS)

myshell : csapp.c cmd.c myshell.c

clean:
	rm -rf *~ $(PROGS)

