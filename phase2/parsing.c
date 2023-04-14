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

/* $begin parseline */
/* parseline - Parse the command line and build the argv array */
void parse_arg(char *sentence, char **argv) 
{
	char *delim;         /* Points to first space delimiter */
	int argc;            /* Number of args */
	int bg;              /* Background job? */

	sentence[strlen(sentence)-1] = ' ';  /* Replace trailing '\n' with space */
	while (*sentence && (*sentence == ' ')) /* Ignore leading spaces */
		sentence++;

	/* Build the argv list */
	argc = 0;
	while ((delim = strchr(sentence, ' '))) {
		argv[argc++] = sentence;
		*delim = '\0';
		sentence = delim + 1;
		while (*sentence && (*sentence == ' ')) /* Ignore spaces */
			sentence++;
	}
	argv[argc] = NULL;
}
/* $end parseline */
