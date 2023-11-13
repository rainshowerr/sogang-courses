#ifndef MYSHELL_H
#define MYSHELL_H

#include "csapp.h"
#define true 		1
#define MAXARGS		128

char	hispath[MAXLINE];

int		is_duplicate(FILE *his, char *cmdline);
void	check_his_cmd(char *cmdline);
void	parse_sentence(char *cmdline, char **sentence, int *pipenum);
void	parse_arg(char *buf, char **argv);
char	**mem_init(int size);

#endif