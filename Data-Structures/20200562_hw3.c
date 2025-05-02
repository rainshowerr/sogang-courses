#include <stdio.h>
#include <stdlib.h>

#define MAX_ELEMENTS 10000000
#define TRUE 1
#define FALSE 0
#define HEAP_FULL(n) (n == MAX_ELEMENTS - 1)
#define HEAP_EMPTY(n) (!n)

typedef struct {
    int start;
    int end;
    int weight;
} element;

void insert_min_heap(element *heap, element item, int *n);
element delete_min_heap(element *heap, int *n);
int simpleFind(int i);
void weightedUnion(int i, int j);
void printResult(element *heap, int *n, FILE *outputFile);

element unselectedEdges[MAX_ELEMENTS];
element selectedEdges[MAX_ELEMENTS];
int unselectedN = 0, selectedN = 0;
int parent[MAX_ELEMENTS];
int vertexNum;

void main(int argc, char *argv[]) {
    FILE *inputFile = fopen(argv[1], "r");
    FILE *outputFile = fopen("20200562_res.txt", "w");
    element item;
    int edgeNum;
    /* 사용자에게 입력받아 unselectedEdges 힙 초기화 */
    fscanf(inputFile, "%d", &vertexNum);
    fscanf(inputFile, "%d", &edgeNum);
    for (int i = 0; i < edgeNum; i++) {
        int start, end, weight;
        fscanf(inputFile, "%d", &start);
        fscanf(inputFile, "%d", &end);
        fscanf(inputFile, "%d", &weight);
        item.start = start;
        item.end = end;
        item.weight = weight;
        insert_min_heap(unselectedEdges, item, &unselectedN);
    }
    /* safe edge인지 check */
    for (int i = 0; i < vertexNum; i++) parent[i] = -1;
    while (selectedN < vertexNum - 1 && unselectedN > 0) {
        item = delete_min_heap(unselectedEdges, &unselectedN);
        if (simpleFind(item.start) != simpleFind(item.end)) {
            insert_min_heap(selectedEdges, item, &selectedN);
            weightedUnion(item.start, item.end);
        }
    }
    printResult(selectedEdges, &selectedN, outputFile);
}

void insert_min_heap(element *heap, element item, int *n) {
    /* insert item into a max heap of current size *n */
    int i;
    if (HEAP_FULL(*n)) {
        fprintf(stderr, "The heap is full.\n");
        exit(1);
    }
    i = ++(*n);
    while ((i != 1) && (item.weight < heap[i / 2].weight)) {
        heap[i] = heap[i / 2];
        i /= 2;
    }
    heap[i] = item;
}

element delete_min_heap(element *heap, int *n) {
    /* delete element with the highest key from the heap */
    int parent, child;
    element item, temp;
    if (HEAP_EMPTY(*n)) {
        fprintf(stderr, "The heap is empty");
        exit(1);
    }
    /* save value of the element with the largest key */
    item = heap[1];
    /* use the last element in the heap to adjust heap */
    temp = heap[(*n)--];
    parent = 1;
    child = 2;
    while (child <= *n) {
        /* find the larger child of the current parent */
        if ((child < *n) && (heap[child].weight > heap[child + 1].weight))
            child++;
        if (temp.weight <= heap[child].weight) break;
        /* move to the next lower level */
        heap[parent] = heap[child];
        parent = child;
        child *= 2;
    }
    heap[parent] = temp;
    return item;
}

int simpleFind(int i) {
    for (; parent[i] >= 0; i = parent[i]);
    return i;
}

void weightedUnion(int i, int j) {
    int temp = parent[i] + parent[j];
    if (i == j) return;
    if (parent[i] > parent[j]) {
        parent[i] = j;
        parent[j] = temp;
    } else {
        parent[j] = i;
        parent[i] = temp;
    }
}

void printResult(element *heap, int *n, FILE *outputFile) {
    element item;
    int isMST = FALSE;
    int weight = 0;
    if (*n == vertexNum - 1)  // cycle이 없으므로 vertex 개수
        isMST = TRUE;
    while (*n > 0) {
        item = delete_min_heap(heap, n);
        weight += item.weight;
        fprintf(outputFile, "%d %d %d\n", item.start, item.end, item.weight);
    }
    fprintf(outputFile, "%d\n", weight);
    if (isMST == TRUE)
        fprintf(outputFile, "CONNECTED\n");
    else
        fprintf(outputFile, "DISCONNECTED\n");
}