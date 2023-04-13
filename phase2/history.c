#include "myshell.h"

int	is_duplicate(FILE *his, char *cmdline) {
	char line[MAXLINE];

	his = Fopen(hispath, "r");
	if (Fgets(line, MAXLINE, his) == NULL) {
        return 0;
    }
	while(Fgets(line, MAXLINE, his) != NULL);
	Fclose(his);
	if (strcmp(line, cmdline) == 0)
		return (1);
	return (0);
}

static int	check_his_line() {
	int		i;
	FILE	*his;
	char	line[MAXLINE];
	his = Fopen(hispath, "r");
	for(i = 0 ; Fgets(line, MAXLINE, his) != NULL ; i++)
		i++;
	Fclose(his);
	return i;
}

void	check_his_cmd(char *cmdline) {
	FILE	*his;
	int		linenum;

	if (cmdline[0] == '!') {
		if (cmdline[1] == '!') {
			his = Fopen(hispath, "r");
			while(Fgets(cmdline, MAXLINE, his) != NULL);
			Fclose(his);
		}
		else if ('1' <= cmdline[1] && cmdline[1] <= '9') {
			cmdline[0] = '+';
			linenum = check_his_line();
			if (atoi(cmdline) > linenum) {
				cmdline[0] = '!';
				return;
			}
			char line[MAXLINE];
			his = Fopen(hispath, "r");
			for (int i = 0; i < atoi(cmdline); i++)
				Fgets(line, MAXLINE, his);
			Fclose(his);
			strcpy(cmdline, line);
		}
	}
}
