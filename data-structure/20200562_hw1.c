#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 100
#define FALSE 0
#define TRUE 1
#define EXIT_ROW 11
#define EXIT_COL 15
#define INVALID_KEY -1

typedef struct {
	short int row;
	short int col;
	short int dir;
	short int key;
} element;

typedef struct {
	short int vert;
	short int horiz;
} offsets;

int top = -1;
element stack[MAX_STACK_SIZE];
offsets move[8] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}}; /* array of moves for each direction */
int mark[13][17] = { 0, }; /* 이미 방문했으면 1, 아니면 0 */
int maze[13][17] = {
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1},
        {1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1},
        {1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1},
        {1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1},
        {1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1},
        {1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1},
        {1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1},
        {1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

// pop, push, stackFull, stackEmpty 구현

element stackEmpty() {  // return an element with invalid values for error detection
	element e;
	e.key = INVALID_KEY;
	return e;
}

void stackFull() { // output error to stderr & exit
	fprintf(stderr, "no more space in the stack\n");
	exit(1);
}

void stackPush(element item) {
/* add an item to the global stack */
	if(top >= MAX_STACK_SIZE - 1)
		stackFull(); /* error handler */
	stack[++top] = item;
}

element stackPop() {
	/* return the top element from the stack */
	if(top == -1)
		return stackEmpty(); /* error handler */
	return stack[top--];
}

/* output a path through the maze if such a path exists */
void path(void) {
	int i, row, col, nextRow, nextCol, dir, found = FALSE;
	element position;
	// mark the start position as visited
	mark[1][1] = 1;
	// push the start position and direction
	top = 0;
	stack[0].row = 1; stack[0].col = 1; stack[0].dir = 3; stack[0].key = 0;
	while(top > -1 && !found) {
		element position = stackPop();
		row = position.row; col = position.col; dir = position.dir;
		while(dir < 8 && !found) {
			/* move in direction dir */
			nextRow = row + move[dir].vert;
			nextCol = col + move[dir].horiz;
			if(nextRow == EXIT_ROW && nextCol == EXIT_COL)
				found = TRUE;
			else if(!maze[nextRow][nextCol] && !mark[nextRow][nextCol]) {
				mark[nextRow][nextCol] = 1;
				position.row = row; position.col = col;
				position.dir = ++dir; position.key = 0;
				stackPush(position);
				row = nextRow; col = nextCol; dir = 0;
			}
			else ++dir;
		}
	}
	if(found) {
		printf("The path is: \n");
		printf("row col\n");
		for(i = 0; i <= top; i++)
			printf("%2d%5d\n", stack[i].row, stack[i].col);
		printf("%2d%5d\n", row, col);
		printf("%2d%5d\n", EXIT_ROW, EXIT_COL);
	}
	else printf("The maze does not have a path.\n");
}

int main() {
	path();
}