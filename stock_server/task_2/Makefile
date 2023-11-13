CC = gcc
CFLAGS=-O2 -Wall
LDLIBS = -lpthread

all: multiclient stockclient stockserver

multiclient: multiclient.c avl_tree.c csapp.c task_2.h
stockclient: stockclient.c avl_tree.c csapp.c task_2.h
stockserver: stockserver.c avl_tree.c echo.c csapp.c task_2.h

clean:
	rm -rf *~ multiclient stockclient stockserver *.o
