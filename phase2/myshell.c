#include "myshell.h"

void go(char **sentence, int pipenum, int cnt);
void eval(char *cmdline);
void execute(char *sentence, char **argv);
int builtin_command(char **argv);

int main(void) {
	char	cmdline[MAXLINE];
	FILE	*his;

	getcwd(hispath, MAXLINE);
	strcat(hispath, "/.history.txt");

	do while (1) {
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
		// Parsing & execute
		eval(cmdline);
	} while (true);
}

/* reculsive하게 명령어의 출력값을 파이프를 통해 입력받으며 명령어를 실행시키는 함수 */
void go(char **sentence, int pipenum, int cnt) {
	int fd[2];
	pid_t pid;
	char buf[MAXLINE];
	char *argv[MAXARGS];

	parse_arg(sentence[cnt], argv);

	if (pipe(fd) == -1)
		exit(1);
	if ((pid = Fork()) == -1)
		exit(1);
	// 자식 프로세스에서 쓰기
	if (pid == 0) {
		close(fd[0]);
		if (cnt == pipenum)
			execute(sentence[cnt], argv);
		else {
			dup2(fd[1], 1); /* 파이프를 통한 출력 */
			close(fd[1]);
			execute(sentence[cnt], argv);
		}
		exit(0);
	}
	// 부모 프로세스에서 읽어들이기
	if (pid > 0) {
		int status;
		waitpid(pid, &status, 0);
		if (cnt < pipenum) {
			close(fd[1]);
			dup2(fd[0], 0); /* 파이프를 통한 입력 */
			close(fd[0]);
			go(sentence, pipenum, cnt + 1);
		}
		exit(0);
	}
}

/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline) 
{
	char **sentence = mem_init(MAXLINE);
	char *argv[MAXARGS];
	char buf[MAXLINE];
	int	pipenum = 0;
	int pid;

	strcpy(buf, cmdline);
	parse_sentence(buf, sentence, &pipenum); /* 파이프 단위로 parsing */
	if (sentence[0] == NULL)
		return;   /* Ignore empty lines */
	pipenum--;
	if (pipenum == 0) { /* pipe가 없을 때 */
		parse_arg(sentence[0], argv);
		execute(sentence[0], argv);
	}
	else { /* piipe가 하나 이상 있을 떄 */
		int pid;
		if ((pid = Fork()) == -1)
			exit(1);
		else if (pid == 0) /* 자식 프로세스 : 명령어를 파이프를 통해 실행시키는 함수 호출 */
			go(sentence, pipenum, 0);
		else if (pid > 1) { /* 부모 프로세스는 wait */
			int status;
			Waitpid(pid, &status, 0);
		}
	}
}
/* $end eval */

/* 명령어를 실행하는 함수 */
void execute(char *sentence, char **argv) {
	int pid;
	if (!builtin_command(argv)) { //quit -> exit(0), & -> ignore, other -> run
		if ((pid = Fork()) == 0) {
			if (execvp(argv[0], argv) < 0) {	//ex) /bin/ls ls -al &
				printf("%s: Command not found.\n", argv[0]);
				exit(0);
			}
		}
		/* Parent waits for foreground job to terminate */
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
