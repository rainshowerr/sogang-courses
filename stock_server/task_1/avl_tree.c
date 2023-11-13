#include "task_1.h"

int getHeight(t_node* node);
int max(int a, int b);
t_node* createNode(t_item item);
void updateHeight(t_node* node);
t_node* rotateRight(t_node* y);
t_node* rotateLeft(t_node* x);
int getBalanceFactor(t_node* node);
t_node* insert(t_node* root, t_item item);
void show_tree(t_node* node, char *buf, int *idx);

t_node *init_tree(FILE *table) {
	t_node *node = NULL;
	char buf[MAXLINE];
	while (Fgets(buf, MAXLINE, table)) {
        t_item item;
		item.ID = atoi(strtok(buf, " "));
		item.left_stock = atoi(strtok(NULL, " "));
		item.price = atoi(strtok(NULL, " "));
		node = insert(node, item);
	}
	return node;
}

// 노드의 높이를 반환하는 함수
int getHeight(t_node* node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// 두 정수 중 더 큰 값을 반환하는 함수
int max(int a, int b) {
    return (a > b) ? a : b;
}

// 새로운 노드를 생성하는 함수
t_node* createNode(t_item item) {
    t_node* newNode = (t_node*)malloc(sizeof(t_node));
    newNode->item = item;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1; // 초기 높이는 1로 설정
    return newNode;
}

// 노드의 높이를 업데이트하는 함수
void updateHeight(t_node* node) {
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
}

// 오른쪽으로 회전하는 함수
t_node* rotateRight(t_node* y) {
    t_node* x = y->left;
    t_node* T2 = x->right;

    // 회전
    x->right = y;
    y->left = T2;

    // 높이 업데이트
    updateHeight(y);
    updateHeight(x);

    return x; // 새로운 루트 반환
}

// 왼쪽으로 회전하는 함수
t_node* rotateLeft(t_node* x) {
    t_node* y = x->right;
    t_node* T2 = y->left;

    // 회전
    y->left = x;
    x->right = T2;

    // 높이 업데이트
    updateHeight(x);
    updateHeight(y);

    return y; // 새로운 루트 반환
}

// 노드의 균형인수를 계산하는 함수
int getBalanceFactor(t_node* node) {
    if (node == NULL)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// AVL 트리에 데이터를 삽입하는 함수
t_node* insert(t_node* root, t_item item) {
    if (root == NULL)
        return createNode(item);

    if (item.ID < root->item.ID)
        root->left = insert(root->left, item);
    else if (item.ID > root->item.ID)
        root->right = insert(root->right, item);
    else // 중복된 값은 허용하지 않음
        return root;

    updateHeight(root); // 높이 업데이트

    int balance = getBalanceFactor(root);

    // 균형을 유지하는 회전 연산 및 재균형 처리
    if (balance > 1 && item.ID < root->left->item.ID) {
        // LL 상황: 왼쪽 자식의 왼쪽에 삽입
        return rotateRight(root);
    }
    if (balance < -1 && item.ID > root->right->item.ID) {
        // RR 상황: 오른쪽 자식의 오른쪽에 삽입
        return rotateLeft(root);
    }
    if (balance > 1 && item.ID > root->left->item.ID) {
        // LR 상황: 왼쪽 자식의 오른쪽에 삽입
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (balance < -1 && item.ID < root->right->item.ID) {
        // RL 상황: 오른쪽 자식의 왼쪽에 삽입
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root; // 변경된 루트 반환
}

// 중위 순회를 사용하여 AVL 트리의 데이터를 버퍼에 담는 함수
void show_tree(t_node* node, char *buf, int *idx) {
	if (node == NULL)
		return;
	show_tree(node->left, buf, idx);
	*idx += snprintf(buf + *idx, MAXBUF, "%d %d %d\n", node->item.ID, node->item.left_stock, node->item.price);
	show_tree(node->right, buf, idx);
}

// 중위 순회를 사용하여 AVL 트리의 데이터를 파일에 출력하는 함수
void fprint_tree(t_node *node, FILE *filename) {
    if (node == NULL)
		return;
	fprint_tree(node->left, filename);
	fprintf(filename, "%d %d %d\n", node->item.ID, node->item.left_stock, node->item.price);
	fprint_tree(node->right, filename);
}

t_node *search(t_node* root, int ID) {
    // 루트가 NULL이거나 탐색할 키와 루트의 키가 일치하면 루트를 반환
    if (root == NULL || root->item.ID == ID)
        return root;
    // 탐색할 키가 루트의 키보다 작으면 왼쪽 서브트리에서 재귀적으로 탐색
    if (ID < root->item.ID)
        return search(root->left, ID);
    // 탐색할 키가 루트의 키보다 크면 오른쪽 서브트리에서 재귀적으로 탐색
    return search(root->right, ID);
}