#include "mp2.h"

/* arr을 heap의 형태로 만드는 함수 */
static void heapify(int *arr, int here, int size) {
	int max = here;
	int left = here * 2 + 1;
	int right = here * 2 + 2;

	/* 자식 중 큰 쪽의 index로 max 갱신*/
	if (left < size && arr[left] > arr[max])
		max = left;
	if (right < size && arr[right] > arr[max])
		max = right;

	/* 자식이 크다면 부모와 swap */
	if (max != here) {
		swap(&arr[here], &arr[max]);
		heapify(arr, max, size); // 재귀적으로 반복
	}
}

static void heap(int *arr, int size) {
	/* heap 생성 */
	for(int i = size / 2 - 1; i >= 0; i--) // 밑에 있는 부모노드부터 역순으로 heapify를 호출해서 최대힙 형태를 완성시킴
		heapify(arr, i, size);
	/* heap 정렬 */
	for(int i = size - 1; i > 0; i--) {
		swap(&arr[0], &arr[i]); // 루트와 정렬되지 않은 노드 중 맨 끝 노드를 swap
		heapify(arr, 0, i); // 정렬
	}
}

void mysort(int *arr, int p, int r, int depth) {
	if (depth == 0)
		heap(arr, r - p + 1);
	else if (p < r) {
		int q;
		q = partition(arr, p, r);
		mysort(arr, p, q - 1, depth - 1);
		mysort(arr, q + 1, r, depth - 1);
	}
}