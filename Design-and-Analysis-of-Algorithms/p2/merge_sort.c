#include "mp2.h"

static void combine(int *arr, int p, int mid, int r) {
	int Lnum = mid - p + 1, Rnum = r - (mid + 1) + 1;

	int *Larr = (int *)malloc(sizeof(int) * Lnum);
	int *Rarr = (int *)malloc(sizeof(int) * Rnum);

	for(int i = p, k = 0; i <= mid; i++, k++)
		Larr[k] = arr[i];
	for(int j = mid + 1, k = 0; j <= r; j++, k++)
		Rarr[k] = arr[j];

	int i = 0, j = 0, k = p;
	while (i < Lnum && j < Rnum) {
		if (Larr[i] <= Rarr[j])
			arr[k] = Larr[i++];
		else
			arr[k] = Rarr[j++];
		k++;
	}

	while (i < Lnum)
		arr[k++] = Larr[i++];

	while (j < Rnum)
		arr[k++] = Rarr[j++];
	free(Larr);
	free(Rarr);
}

void merge(int *arr, int p, int r) {
	if (p < r) {
		int mid = (p + r) / 2;
		merge(arr, p, mid);
		merge(arr, mid + 1, r);
		combine(arr, p, mid, r);
	}
}