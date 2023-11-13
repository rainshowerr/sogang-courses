#include "myshell.h"

void eval(char *cmdline);
void eval(char *cmdline);
void execute(int bg, char *cmdline, char **argv);
int builtin_command(char **argv); 

int main(void) {
	char	cmdline[MAXLINE];
	FILE	*his;

	if (!getcwd(hispath, MAXLINE)) {
		printf("Failed to allocate hispath\n");
		return (0);
	}
	strcat(hispath, "/.history.txt");

	do{
		printf("CSE4100-MP-P1>");
		// Reading: Read the command from standard input.
		Fgets(cmdline, MAXLINE, stdin);
		if (feof(stdin))
			exit(0);
		check_his_cmd(cmdline);
		// history에 추가해도 되는지 확인 후 추가
		if (!is_duplicate(his, cmdline) && cmdline[0] != '!') {
			his = Fopen(hispath, "a");
			Fputs(cmdline, his);
			Fclose(his);
		}
		// Parsing: transform the input string into command line arguments.
		eval(cmdline);
	} while (true);
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
	parse_argv(buf, argv); /* 파싱 */
	if (argv[0] == NULL)  
		return;   /* Ignore empty lines */
	execute(bg, cmdline, argv); /* 명령어 실행 */
}

/* 명령어를 실행하는 함수 */
void execute(int bg, char *cmdline, char **argv) {
	int pid;
	if (!builtin_command(argv)) { //quit -> exit(0), & -> ignore, other -> run
		if ((pid = Fork()) == 0) {
			if (execvp(argv[0], argv) < 0) {	//ex) /bin/ls ls -al &
				printf("%s: Command not found.\n", argv[0]);
				exit(0);
			}
		}
		int status;
		Waitpid(pid, &status, 0);
	}
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
	if (!strcmp(argv[0], "cd")) { /* cd command */
		if (chdir(argv[1]))
			printf("failed to change directory");
		return 1;
	}
	return 0;                     /* Not a builtin command */
}
/* $end eval */
