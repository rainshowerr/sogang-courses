#include "myshell.h"

/* 왼쪽 또는 오른쪽부터 따옴표 개수를 세는 함수 */
static int cnt_quote(char *argv, char c) {
    int cnt = 0;
    if (c == 'l') { /* 왼 */
        for(int i = 0; i < strlen(argv); i++) {
            if (argv[i] == '\"')
                cnt++;
            else
                break;
        }
    }
    else if (c == 'r') { /* 오 */
        for(int i = strlen(argv) - 1; 0 <= i; i--) {
            if (argv[i] == '\"')
                cnt++;
            else
                break;
        }
    }
    return cnt;
}

// 따옴표를 제거하는 함수
static void del_quote(char **argv) {
    int left = 0, right = 0;
    for (int i = 0; argv[i]; i++) {
        left = cnt_quote(argv[i], 'l');
        right = cnt_quote(argv[i], 'r');
        if (left == right) {
            *(argv[i] + strlen(argv[i]) - right) = '\0';
            argv[i] = argv[i] + left;
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