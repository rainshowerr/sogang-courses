/* 
 * echoserveri.c - An iterative echo server 
 */ 
/* $begin echoserverimain */
#include "task_1.h"

void init_pool(int listenfd, pool *p);
void add_client(int connfd, pool *p);
void check_clients(pool *p);
int is_all_disconnected(pool *p);
void save_stock_table();
void sigint_handler(int signo);

t_node *node;

int main(int argc, char **argv) 
{

	Signal(SIGINT, sigint_handler);

	FILE* stock_table;
	int listenfd, connfd;
	socklen_t clientlen;
	struct sockaddr_storage clientaddr;  /* Enough space for any address */  //line:netp:echoserveri:sockaddrstorage
	char client_hostname[MAXLINE], client_port[MAXLINE];
	static pool pool;

	if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(0);
	}

	stock_table = Fopen("stock.txt", "r");
	node = init_tree(stock_table);
	Fclose(stock_table);

	listenfd = Open_listenfd(argv[1]);
	init_pool(listenfd, &pool);
	while (1) {
		/* Wait for listening/connectid descriptor(s) to become ready */
		pool.ready_set = pool.read_set;
		pool.nready = Select(pool.maxfd+1, &pool.ready_set, NULL, NULL, NULL);

		/* If listening descriptor ready, add new client to pool */
		if (FD_ISSET(listenfd, &pool.ready_set)) {
			clientlen = sizeof(struct sockaddr_storage);
			connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
			Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
			printf("Connected to (%s, %s)\n", client_hostname, client_port);
			add_client(connfd, &pool);
		}
		/* Echo a text line from each ready connected descriptor */
		check_clients(&pool);
	}
	exit(0);
}
/* $end echoserverimain */

void init_pool(int listenfd, pool *p)
{
	/* Initially, there are no connected descriptors */
	int i;
	p->maxi = -1;
	for (i = 0; i < FD_SETSIZE; i++)
		p->clientfd[i] = -1;

	/*Initially, listenfd is only member of select read set */
	p->maxfd = listenfd;
	FD_ZERO(&p->read_set);
	FD_SET(listenfd, &p->read_set);
}

void add_client(int connfd, pool *p)
{
	int i;
	p->nready--;
	for (i = 0; i < FD_SETSIZE; i++) /* Find an available slot*/
		if (p->clientfd[i] < 0) {
			/* Add connected descriptor to the pool */
			p->clientfd[i] = connfd;
			Rio_readinitb(&p->clientrio[i], connfd);

			/* Add the descriptor to descriptor set */
			FD_SET(connfd, &p->read_set);

			/* Update max descriptor and pool high water mark */
			if (connfd > p->maxfd)
				p->maxfd = connfd;
			if (i > p->maxi)
				p->maxi = i;
			break;
		}
	if (i == FD_SETSIZE) /* Couldn't find an empty slot */
		app_error("add_client error: Too many clients");
}

void check_clients(pool *p)
{
	int i, connfd, n;
	rio_t rio;
	t_node *stock;
	char buf[MAXLINE];

	for (i = 0; (i <= p->maxi) && (p->nready > 0); i++) {
		connfd = p->clientfd[i];
		rio = p->clientrio[i];

		/* If the descriptor is ready, echo a text line from it */
		if ((connfd > 0) && FD_ISSET(connfd, &p->ready_set)) {
			p->nready--;
			if ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
				printf("server received %d bytes\n", n);
				char *command = strtok(buf, " \n");
				if (!strcmp(command, "show"))
				{
					int idx = 0;
					char showbuf[MAXBUF];
					show_tree(node, showbuf, &idx);
					Rio_writen(connfd, showbuf, MAXLINE);
				}
				else if (!strcmp(command, "buy")) {
					int ID = atoi(strtok(NULL, " \n"));
					stock = search(node, ID);
					int buy_cnt = atoi(strtok(NULL, " \n"));
					if (stock->item.left_stock >= buy_cnt) {
						stock->item.left_stock -= buy_cnt;
						Rio_writen(connfd, "[buy] success\n", MAXBUF);
					}
					else
						Rio_writen(connfd, "Not enough left stocks\n", MAXBUF);
				}
				else if (!strcmp(command, "sell")) {
					int ID = atoi(strtok(NULL, " \n"));
					stock = search(node, ID);
					stock->item.left_stock += atoi(strtok(NULL, " \n"));
					Rio_writen(connfd, "[sell] success\n", MAXBUF);
				}
				else if (!strcmp(command, "exit")) {
					Close(connfd);
					FD_CLR(connfd, &p->read_set);
					p->clientfd[i] = -1;
					if (is_all_disconnected(p))
						save_stock_table();
				}
			}
			/*EOF detected, removes descriptor from pool */
			else {
				Close(connfd);
				FD_CLR(connfd, &p->read_set);
				p->clientfd[i] = -1;
				if (is_all_disconnected(p))
					save_stock_table();
			}
		}
	}
}

int is_all_disconnected(pool *p) {
	for(int i = 0; (i <= p->maxi); i++) {
		if (p->clientfd[i] != -1)
			return (0);
	}
	return (1);
}

void    save_stock_table() {
	FILE *stock_table = Fopen("stock.txt", "w");
	fprint_tree(node, stock_table);
	Fclose(stock_table);
}

void sigint_handler (int signo){
	save_stock_table();
	exit(1);
}