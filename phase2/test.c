#include "myshell.h"

int parse_sentence(char *cmdline, char **sentence, int *pipenum) {
    int bg, cnt = 0;
    char *delim;
    char line[MAXLINE];

    cmdline[strlen(cmdline) - 1] = '|';
    while (*cmdline && *cmdline == ' ')
        cmdline++;
    while (delim = strchr(cmdline, '|')) {
        (*pipenum)++;
        *delim = '\0';
        sentence[cnt] = cmdline;
        cmdline = delim + 1;
        while (*cmdline && (*cmdline == ' ')) /* Ignore spaces */
            cmdline++;
        cnt++;
    }
    for(int i = 0; sentence[i]; i++)
        strcat(sentence[i], "\n");
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
	char **arr = (char **)malloc(sizeof(char *) * size);
	int	i = 0;

	while (i < size)
	{
		arr[i] = (char *)malloc(sizeof(char) * size);
		i++;
	}
	return (arr);
}

int main() {
    char cmdline[MAXLINE] = "ls -l | wc -l\n";
    char **sentence;
    char *argv[MAXARGS];
    int pipenum = 0;

    parse_sentence(cmdline, sentence, &pipenum);
    printf("%s\n", sentence[0]);
    printf("%s\n", sentence[1]);
    printf("%d", pipenum);
    //parse_arg(sentence[1], argv);
    //printf("%s %s %s\n", argv[0], argv[1], argv[2]);
}