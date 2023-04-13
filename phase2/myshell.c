#include "myshell.h"

void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv); 

int main(void) {
	char	cmdline[MAXLINE];
	FILE	*his;

	getcwd(hispath, MAXLINE);
	strcat(hispath, "/.history.txt");
	do{
		printf("CSE4100-MP-P1>");
		// Reading: Read the command from standard input.
		Fgets(cmdline, MAXLINE, stdin);
		if (feof(stdin))
			exit(0);
		check_his_cmd(cmdline);
		if (!is_duplicate(his, cmdline) && cmdline[0] != '!') {
			his = Fopen(hispath, "a");
			Fputs(cmdline, his);
			Fclose(his);
		}
		// Parsing: transform the input string into command line arguments.
		eval(cmdline);
	} while (true);
}

void execute(int bg, char **argv, char *sentence) {
	pid_t pid;           /* Process id */

	bg = parseline(sentence, argv); 
	if (argv[0] == NULL)  
		return;   /* Ignore empty lines */
	if (!builtin_command(argv)) { //quit -> exit(0), & -> ignore, other -> run
		if ((pid = Fork()) == 0) {
			if (execvp(argv[0], argv) < 0) {	//ex) /bin/ls ls -al &
				printf("%s: Command not found.\n", argv[0]);
				exit(0);
			}
		}
		// 이거 여기있음 안될거같은데 내일 생각해보자..
		/* Parent waits for foreground job to terminate */
		if (!bg){ 
			int status;
			if (waitpid(pid, &status, 0) < 0)
				unix_error("waitpid error");
		}
		else //when there is backgrount process!
			printf("%d %s", pid, sentence);
	}
}



/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline) 
{
	char *argv[MAXARGS]; /* Argument list execve() */
	char buf[MAXLINE];   /* Holds modified command line */
	char *sentence[MAXLINE];
	int bg;              /* Should the job run in bg or fg? */
	
	parse_sentence(cmdline, sentence);
	// execute(argv, sentence[0]);
	return;
}
/* $end eval */


/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv) 
{
	if (!strcmp(argv[0], "exit")) /* quit command */
		exit(0);
	if (!strcmp(argv[0], "history")) { /* history command */
		FILE *his;
		char line[MAXLINE];
		his = Fopen(hispath, "r");
		while (fgets(line, MAXLINE, his) != NULL)
			printf("%s", line);
		fclose(his);
		return 1;
	}
	if (!strcmp(argv[0], "cd")) {
		if (chdir(argv[1]))
			printf("failed to change directory");
		return 1;
	}
	return 0;                     /* Not a builtin command */
}

int parse_sentence(char *cmdline, char **sentence) {
    int bg, cnt = 0;
    char *delim;

    cmdline[strlen(cmdline) - 1] = '|';
    while (*cmdline && *cmdline == ' ')
        cmdline++;
    cnt = 0;
    while(delim = strchr(cmdline, '|')) {
        sentence[cnt++] = cmdline;
        *delim = '\0';
        cmdline = delim + 1;
        while (*cmdline && (*cmdline == ' ')) /* Ignore spaces */
            cmdline++;
    }
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

