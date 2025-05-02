#include "task_1.h"

int main(){
	char buf[8192];
	strcpy(buf, "[buy] success\n");
	Rio_writen(1, buf, strlen(buf));
}