#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int **makeSumarr(int **input, int row, int col) {
	int **sum = (int **)malloc(sizeof(int *) * (row + 1));
	for(int i = 0; i <= row; i++) {
		sum[i] = (int *)malloc(sizeof(int) * (col + 1));
		memset(sum[i], 0, sizeof(int)* (col + 1)); /* 0으로 초기화 */
	}

    for (int i = 1; i <= row; i++) {
        for (int j = 1; j <= col; j++) {
            sum[i][j] = input[i][j] + sum[i - 1][j] + sum[i][j - 1] - sum[i - 1][j - 1];
        }
    }
	return sum;
}

int O6(int **input, int row, int col) {
	int i, j, a, b, x, y, max = input[1][1];

	/* 직사각형의 시작 인덱스 지정 */
	for(i = 1; i <= row; i++) {
		for(j = 1; j <= col; j++) {
			/* 직사각형의 끝 인덱스 지정 */
			for(a = i; a <= row; a++) {
				for(b = j; b <= col; b++) {
					/* 합 계산 */
					int temp = 0;
					for(x = i; x <= a; x++) {
						for(y = j; y <= b; y++) {
							temp += input[x][y];
						}
					}
					/* 업데이트 */
					if (temp > max)
						max = temp;
				}
			}
		}
	}
	return max;
}

int O4(int **input, int row, int col) {
	int i, j, a, b, max = input[1][1];;

	int **sum = makeSumarr(input, row, col);
	/* 직사각형의 시작 인덱스 지정 */
	for(i = 1; i <= row; i++) {
		for(j = 1; j <= col; j++) {
			/* 직사각형의 마지막 인덱스 지정 */
			for(a = i; a <= row; a++) {
				for(b = j; b <= col; b++) {
					int temp = sum[a][b] - sum[i - 1][b] - sum[a][j - 1] + sum[i - 1][j - 1];
					/* 업데이트 */
					if (temp > max)
						max = temp;
				}
			}
		}
	}
	free(sum);
	return max;
}

int O3(int **input, int row, int col) {
	int max = input[1][1];

	/* 직사각형 시작 위치와 col 사이즈 지정 */
	int **sum = makeSumarr(input, row, col);
	for(int j = 1; j <= col; j++) { /* 시작지점 col 좌표 */
		for(int k = 0; k <= col - j; k++) {  /* k는 col의 좌표를 오른쪽으로 이동시키는 인덱스 */
			int temp = 0;
			for(int i = 1; i <= row; i++) { /* 시작지점 row 좌표 */
				temp += sum[i][j + k] - sum[i - 1][j + k] - sum[i][j - 1] + sum[i - 1][j - 1];
				if (temp < 0) /* 직사각형의 합이 음수면 버리고 새로시작*/
					temp = 0;
				else if (temp > max) /* 업데이트 */
					max = temp;
			}
		}
	}
	free(sum);
	return max;
}

int main(int argc, char **argv){
	int row, col, max;
	clock_t start, end;

	/* 입력 */
	FILE *inputfile = fopen(argv[1], "r");
	fscanf(inputfile, "%d %d", &row, &col);
	int **input = (int **)malloc(sizeof(int *) * (row + 1));
	for(int i = 0; i <= row; i++)
		input[i] = (int *)malloc(sizeof(int) * (col + 1));
	for(int i = 1; i <= row; i++) {
		for(int j = 1; j <= col; j++)
			fscanf(inputfile, "%d", &input[i][j]);
	}
	fclose(inputfile);

	start = clock();

	/* 계산 알고리즘 */
	if (atoi(argv[2]) == 1)
		max = O6(input, row, col);
	else if (atoi(argv[2]) == 2)
		max = O4(input, row, col);
	else if (atoi(argv[2]) == 3)
		max = O3(input, row, col);
	free(input);

	end = clock();

	/* 출력 */
	FILE *outputfile = fopen(strcat(strdup("result_"), argv[1]), "w");
	fprintf(outputfile, "%s\n%s\n%d\n%d\n%d\n%ld", argv[1], argv[2], row, col, max, end-start);
	fclose(outputfile);
}