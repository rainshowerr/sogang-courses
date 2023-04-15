#include "myshell.h"

int parse_sentence(char *buf, char **sentence, int *pipenum) {
    int bg, cnt = 0;
    char *delim;
    size_t  len = strlen(buf);

    // substitute \n with pipe
    buf[strlen(buf) - 1] = '|';
    // push spaces
    while (*buf && *buf == ' ')
        buf++;
    while (delim = strchr(buf, '|')) {
		(*pipenum)++;
        *delim = '\0';
        strcpy(sentence[cnt], buf);
        strcat(sentence[cnt], "\n");
        cnt++;
        buf = delim + 1;
        while (*buf && (*buf == ' ')) /* Ignore spaces */
             buf++;
    }
    sentence[cnt] = NULL;
	if (cnt == 0)  /* Ignore blank line */
		return 1;
	/* Should the job run in the background? */
    if ((bg = (*sentence[cnt-1] == '&')) != 0)
		sentence[--cnt] = NULL;
    return bg;
}

void parse_arg(char *buf, char **argv) 
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
}
/* $end parseline */

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
