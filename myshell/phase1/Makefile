CC = gcc
CFLAGS = -Og
LDLIBS = -lpthread
PROGS = myshell

all: $(PROGS)

myshell : csapp.c history.c parsing.c myshell.c

clean:
	rm -rf *~ $(PROGS)
