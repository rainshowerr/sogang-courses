#include "mp2.h"

int main(int argc, char **argv) {
	clock_t start, end;
	int num;

	/* 입력 */
	FILE *inputfile = fopen(argv[1], "r");
	fscanf(inputfile, "%d", &num);
	int *arr = (int *)malloc(sizeof(int) * num);
	for(int i = 0; i < num; i++)
		fscanf(inputfile, "%d", &arr[i]);
	fclose(inputfile);

	/* 정렬 */
	start = clock();

	if (atoi(argv[2]) == 1)
		selection(arr, num);
	else if (atoi(argv[2]) == 2)
		quick(arr, 0, num - 1);
	else if (atoi(argv[2]) == 3)
		merge(arr, 0, num - 1);
	else
		mysort(arr, 0, num - 1, 2 * log(num)/log(2));

	end = clock();

	/* 출력 */
	char *outputfileName = (char *)malloc(sizeof(char) * (strlen(argv[1]) + strlen(argv[2]) + 9));
	strcpy(outputfileName, "result_");
	strcat(outputfileName, argv[2]);
	strcat(outputfileName, "_");
	strcat(outputfileName, argv[1]);
	FILE *outputfile = fopen(outputfileName, "w");
	free(outputfileName);
	fprintf(outputfile, "%s\n%s\n%d\n%f\n", argv[1], argv[2], num, (float)(end - start)/CLOCKS_PER_SEC);
	for(int i = 0; i < num; i++)
		fprintf(outputfile, "%d ", arr[i]);
	fclose(outputfile);
	free(arr);
}