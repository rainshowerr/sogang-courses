#include "myshell.h"

/* history 중복 체크 */
int	is_duplicate(FILE *his, char *cmdline) {
	char line[MAXLINE];

	his = Fopen(hispath, "r");
	if (Fgets(line, MAXLINE, his) == NULL) {
        return 0;
    }
	while(Fgets(line, MAXLINE, his) != NULL); /* 마지막 줄 읽기 */
	Fclose(his);
	if (strcmp(line, cmdline) == 0) /* 마지막 줄과 비교 */
		return (1);
	return (0);
}

/* history 명령어 줄 수를 세는 함수 */
static int	check_his_line() {
	int		i = 0;
	FILE	*his;
	char	line[MAXLINE];

	his = Fopen(hispath, "r");
	while (Fgets(line, MAXLINE, his) != NULL)
		i++;
	Fclose(his);
	return i;
}

/* !! 또는 !# 명령어를 처리하는 함수 */
void	check_his_cmd(char *cmdline) {
	FILE	*his;
	int		linenum = check_his_line();

	if (cmdline[0] == '!') {
		if (cmdline[1] == '!') { /* !! 를 history의 마지막 명령어로 대체 */
			his = Fopen(hispath, "r");
			while (Fgets(cmdline, MAXLINE, his) != NULL);
			Fclose(his);
			printf("%s", cmdline);
		}
		else if ('1' <= cmdline[1] && cmdline[1] <= '9') { /* !# 구현 */
			cmdline[0] = '+';
			if (atoi(cmdline) > linenum) { /* history의 명령어 수보다 큰 수가 들어왔을 때 예외처리 */
				cmdline[0] = '!';
				return;
			}
			char line[MAXLINE];
			his = Fopen(hispath, "r");
			for (int i = 0; i < atoi(cmdline); i++) /* #번째 라인 읽기 */
				Fgets(line, MAXLINE, his);
			Fclose(his);
			strcpy(cmdline, line);
			printf("%s", cmdline);
		}
	}
}
