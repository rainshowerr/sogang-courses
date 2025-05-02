#include "mp2.h"

int partition(int *arr, int p, int r) {
	int pivot = arr[r];
	int i = p - 1;
	for (int j = p; j < r; j++) {
		if (arr[j] <= pivot) { // 작은 수를 왼쪽으로 보냄
			i++;
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[r]); // 피봇 위치 가운데로 재설정
	return (i + 1);
}

void quick(int *arr, int p, int r) {
	int q;
	if (p < r) {
		q = partition(arr, p, r);
		quick(arr, p, q - 1);
		quick(arr, q + 1, r);
	}
}