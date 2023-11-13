#ifndef TASK_2_H
#define TASK_2_H

#include "csapp.h"

#define NTHREADS 30
#define SBUFSIZE 30

typedef struct s_item{
    int ID;
    int left_stock;
    int price;
} t_item;

typedef struct s_node {
    t_item item;
    struct s_node *left;
    struct s_node *right;
    int height;
    int readcnt;
    sem_t mutex_readcnt;
    sem_t mutex_write;
} t_node;

typedef struct {
    int *buf; /* Buffer array */
    int n; /*Maximum number of slots */
    int front; /* buf[(front+1)%n] is the first item */
    int rear; /* buf[rear%n] is the last item */
    sem_t mutex; /* Protects accesses to buf */
    sem_t slots; /* Counts available slots */
    sem_t items; /* Counts available items */
} sbuf_t;

void echo(int connfd);

t_node *init_tree(FILE *table);
void show_tree(t_node* node, char *buf, int *idx);
void fprint_tree(t_node *node, FILE *filename);
t_node *search(t_node* root, int ID);

#endif