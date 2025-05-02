/* 
 * echoserveri.c - An iterative echo server 
 */ 
/* $begin echoserverimain */
#include "task_2.h"

int is_all_disconnected();
void save_stock_table();
void *thread(void *vargp);
void sbuf_init(sbuf_t *sp, int n);
void sbuf_insert(sbuf_t *sp, int item);
int sbuf_remove(sbuf_t *sp);
void echo_cnt(int connfd);
void sigint_handler(int signo);

t_node *node;
sbuf_t sbuf; /* Shared buffer of connected descriptors */

int main(int argc, char **argv) 
{

	Signal(SIGINT, sigint_handler);

	FILE* stock_table;
	int i, listenfd, connfd;
	socklen_t clientlen;
	struct sockaddr_storage clientaddr;  /* Enough space for any address */  //line:netp:echoserveri:sockaddrstorage
	char client_hostname[MAXLINE], client_port[MAXLINE];
	pthread_t tid;

	if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(0);
	}

	stock_table = Fopen("stock.txt", "r");
	node = init_tree(stock_table);
	Fclose(stock_table);

	listenfd = Open_listenfd(argv[1]);
	sbuf_init(&sbuf, SBUFSIZE);
	for(i = 0; i < NTHREADS; i++) /* Create a pool of worker threads */
		Pthread_create(&tid, NULL, thread, NULL);
	while (1) {
		clientlen = sizeof(struct sockaddr_storage);
		connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
		Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
		printf("Connected to (%s, %s)\n", client_hostname, client_port);
		sbuf_insert(&sbuf, connfd); /* Insert connfd in buffer */
	}
}
/* $end echoserverimain */

void *thread(void *vargp) {
	Pthread_detach(pthread_self());
	while(1) {
		int connfd = sbuf_remove(&sbuf); /* Remove connfd from buf */
		echo_cnt(connfd); /* Service client */
		Close(connfd);
	}
}

void	sbuf_init(sbuf_t *sp, int n)
{
	sp->buf = Calloc(n, sizeof(int));
	sp->n = n; /* Buffer holds max of n items */
	sp->front = sp->rear = 0; /* Empty buffer if front == rear */
	Sem_init(&sp->mutex, 0, 1); /* Binary semaphore for locking*/
	Sem_init(&sp->slots, 0, n); /* Initially, buf has n empty slots */
	Sem_init(&sp->items, 0, 0); /* Initially, buf has 0 items */
}

/* Clean up buffer sp */
void sbuf_deinit(sbuf_t *sp)
{
	Free(sp->buf);
}

void sbuf_insert(sbuf_t *sp, int item)
{
	P(&sp->slots); /* Wait for available slot */
	P(&sp->mutex); /* Lock the buffer*/
	sp->buf[(++sp->rear)%(sp->n)] = item; /* Insert item */
	V(&sp->mutex); /* Unlock the buffer */
	V(&sp->items); /* Announce available item */
}

int sbuf_remove(sbuf_t *sp)
{
	int item;
	P(&sp->items); /* Wait for available item */
	P(&sp->mutex); /* Lock the buffer */
	item = sp->buf[(++sp->front)%(sp->n)]; /*remove the item */
	V(&sp->mutex);
	V(&sp->slots); /* Announce available slot */
	return (item);
}

void echo_cnt(int connfd)
{
	int n;
	char buf[MAXLINE];
	t_node *stock;
	rio_t rio;

	Rio_readinitb(&rio, connfd);
	while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
		printf("server received %d bytes\n", n);
		char *command = strtok(buf, " \n");
		if (!strcmp(command, "show"))
		{
			int idx = 0;
			char showbuf[MAXBUF];
			show_tree(node, showbuf, &idx);
			Rio_writen(connfd, showbuf, MAXBUF);
		}
		else if (!strcmp(command, "buy")) {
			int ID = atoi(strtok(NULL, " \n"));
			stock = search(node, ID);
			int buy_cnt = atoi(strtok(NULL, " \n"));
			if (stock->item.left_stock >= buy_cnt) {
				P(&node->mutex_readcnt);
				node->readcnt++;
				if (node->readcnt == 1)
					P(&node->mutex_write);
				V(&node->mutex_readcnt);

				stock->item.left_stock -= buy_cnt;
				
				P(&node->mutex_readcnt);
				node->readcnt--;
				if (node->readcnt == 0)
					V(&node->mutex_write);
				V(&node->mutex_readcnt);
				Rio_writen(connfd, "[buy] success\n", MAXBUF);
			}
			else
				Rio_writen(connfd, "Not enough left stocks\n", MAXBUF);
		}
		else if (!strcmp(command, "sell")) {
			int ID = atoi(strtok(NULL, " \n"));
			stock = search(node, ID);
			P(&node->mutex_readcnt);
			node->readcnt++;
			if (node->readcnt == 1)
				P(&node->mutex_write);
			V(&node->mutex_readcnt);

			stock->item.left_stock += atoi(strtok(NULL, " \n"));

			P(&node->mutex_readcnt);
			node->readcnt--;
			if (node->readcnt == 0)
				V(&node->mutex_write);
			V(&node->mutex_readcnt);
			Rio_writen(connfd, "[sell] success\n", MAXBUF);
		}
		else if (!strcmp(command, "exit")) {
			save_stock_table();
			exit(0);
		}
	}
	save_stock_table();
}

void save_stock_table() {
	FILE *stock_table = Fopen("stock.txt", "w");
	fprint_tree(node, stock_table);
	Fclose(stock_table);
}

void sigint_handler(int signo) {
	save_stock_table();
	exit(1);
}