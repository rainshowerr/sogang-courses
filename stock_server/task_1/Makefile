CC = gcc
CFLAGS=-O2 -Wall
LDLIBS = -lpthread #-fsanitize=address

all: multiclient stockclient stockserver

multiclient: multiclient.c avl_tree.c csapp.c task_1.h #client에도 트리파일 필요한가?
stockclient: stockclient.c avl_tree.c csapp.c task_1.h
stockserver: stockserver.c avl_tree.c echo.c csapp.c task_1.h

clean:
	rm -rf *~ multiclient stockclient stockserver *.o
