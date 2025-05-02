#ifndef TASK_1_H
#define TASK_1_H

#include "csapp.h"

typedef struct s_item{
    int ID;
    int left_stock;
    int price;
    sem_t mutex;
} t_item;

typedef struct s_node {
    t_item item;
    struct s_node *left;
    struct s_node *right;
    int height;
} t_node;

typedef struct {
	int maxfd;
    fd_set read_set;
    fd_set ready_set;
    int nready;
    int maxi;
    int clientfd[FD_SETSIZE];
    rio_t clientrio[FD_SETSIZE];
} pool;

void echo(int connfd);

t_node *init_tree(FILE *table);
void show_tree(t_node* node, char *buf, int *idx);
void fprint_tree(t_node *node, FILE *filename);
t_node *search(t_node* root, int ID);

#endif