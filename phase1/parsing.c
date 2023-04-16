#include "myshell.h"

/* 따옴표를 제거하는 함수 */
static void del_quote(char **argv) {
	for(int i = 0; argv[i]; i++) {
		while (1) {
			if (argv[i][0] == '\'' && argv[i][strlen(argv[i]) - 1] == '\'') {
				argv[i][strlen(argv[i]) - 1] = '\0';
				argv[i] = argv[i] + 1;
			}
			else if (argv[i][0] == '\"' && argv[i][strlen(argv[i]) - 1] == '\"') {
				argv[i][strlen(argv[i]) - 1] = '\0';
				argv[i] = argv[i] + 1;
			}
			else
				break;
		}
	}
}

/* $begin parse_argv */
/* parse_argv - Parse the command line and build the argv array */
void parse_argv(char *buf, char **argv) 
{
	char *delim;         /* Points to first space delimiter */
	int argc;            /* Number of args */
	int bg;              /* Background job? */

	buf[strlen(buf)-1] = ' ';  /* Replace trailing '\n' with space */
	while (*buf && (*buf == ' ')) /* Ignore leading spaces */
		buf++;

	/* Build the argv list */
	argc = 0;
	while ((delim = strchr(buf, ' '))) {
		argv[argc++] = buf;
		*delim = '\0';
		buf = delim + 1;
		while (*buf && (*buf == ' ')) /* Ignore spaces */
			buf++;
	}
	argv[argc] = NULL;

	del_quote(argv); /* 따옴표 제거 */
}
/* $end parse_argv */