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

void execute(int bg, char *cmdline, char **argv) {
	int pid;

	if (!builtin_command(argv)) { //quit -> exit(0), & -> ignore, other -> run
		if ((pid = Fork()) == 0) {
			if (execvp(argv[0], argv) < 0) {	//ex) /bin/ls ls -al &
				printf("%s: Command not found.\n", argv[0]);
				exit(0);
			}
		}
		/* Parent waits for foreground job to terminate */
		if (!bg){ 
			int status;
			Waitpid(pid, &status, 0);
		}
	}
}

/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline) 
{
	char *argv[MAXARGS]; /* Argument list execve() */
	char buf[MAXLINE];   /* Holds modified command line */
	int bg;              /* Should the job run in bg or fg? */
	pid_t pid;           /* Process id */
	
	strcpy(buf, cmdline);
	bg = parseline(buf, argv); 
	if (argv[0] == NULL)  
		return;   /* Ignore empty lines */
	execute(bg, cmdline, argv);
}

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
/* $end eval */

/* $begin parseline */
/* parseline - Parse the command line and build the argv array */
int parseline(char *buf, char **argv) 
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

	if (argc == 0)  /* Ignore blank line */
		return 1;

    /* Should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0)
		argv[--argc] = NULL;

    return bg;
}
/* $end parseline */

