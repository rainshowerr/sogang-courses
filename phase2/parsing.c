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

/* 파이프 단위로 잘라진 문장 배열을 만드는 함수*/
void parse_sentence(char *buf, char **sentence, int *pipenum) {
    int cnt = 0;
    char *delim;
    size_t  len = strlen(buf);

    buf[strlen(buf) - 1] = '|'; /* substitute \n with pipe */
    while (*buf && *buf == ' ') /* push space */
        buf++;
    while (delim = strchr(buf, '|')) { /* 각 배열에 파이프 단위 문장 할당 */
		(*pipenum)++;
        *delim = '\0'; /* |를 \0으로 바꿔줌 */
        strcpy(sentence[cnt], buf);
        strcat(sentence[cnt], "\n"); /* 나중에 parse_arg에 들어가기 때문에 개행 주가*/
        cnt++;
        buf = delim + 1;
        while (*buf && (*buf == ' ')) /* Ignore spaces */
             buf++;
    }
    sentence[cnt] = NULL;
}

/* $begin parse_argv */
/* parse_argv - Parse the command line and build the argv array */
void parse_arg(char *buf, char **argv) 
{
	char *delim;         /* Points to first space delimiter */
	int argc;            /* Number of args */

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

	del_quote(argv);
}
/* $end parseline */

/* 2차원 동적 배열을 할당하는 함수 (sentence배열 초기화에 쓰임) */
char **mem_init(int size)
{
	char **arr = (char **)malloc(sizeof(char *) * size + 1);
	int	i = 0;

	while (i < size)
	{
		arr[i] = (char *)malloc(sizeof(char) * size + 1);
		i++;
	}
	return (arr);
}
