#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 100
#define MAX_QUEUE_SIZE 100

struct node {
    char data;
    struct node *left_child, *right_child;
};
typedef struct node *tree_pointer;

tree_pointer queue[MAX_QUEUE_SIZE];

tree_pointer create_tree_node(char data);
void recursive_inorder(tree_pointer ptr);
void iter_inorder(tree_pointer ptr);
void iter_postorder(tree_pointer ptr);
void iter_preorder(tree_pointer ptr);
void level_order(tree_pointer ptr);

/* TODO: add function prototype if necessary */
void addq(int front, int *rear, tree_pointer ptr);
tree_pointer deleteq(int *front, int rear);

void main() {
    /* create a tree that represents an arithmetic expression */
    tree_pointer ptr, ptr1, ptr2;

    ptr1 = create_tree_node('A');
    ptr2 = create_tree_node('B');
    ptr = create_tree_node('/');
    ptr->left_child = ptr1;
    ptr->right_child = ptr2;

    ptr1 = ptr;
    ptr2 = create_tree_node('C');
    ptr = create_tree_node('*');
    ptr->left_child = ptr1;
    ptr->right_child = ptr2;

    ptr1 = ptr;
    ptr2 = create_tree_node('D');
    ptr = create_tree_node('*');
    ptr->left_child = ptr1;
    ptr->right_child = ptr2;

    ptr1 = ptr;
    ptr2 = create_tree_node('E');
    ptr = create_tree_node('+');
    ptr->left_child = ptr1;
    ptr->right_child = ptr2;

    /* call traversal functions */
    recursive_inorder(ptr);
    printf("\n");
    iter_inorder(ptr);
    printf("\n");
    iter_preorder(ptr);
    printf("\n");
    iter_postorder(ptr);
    printf("\n");
    level_order(ptr);
    printf("\n");
}

tree_pointer create_tree_node(char data) {
    tree_pointer ptr = (tree_pointer)malloc(sizeof(struct node));
    ptr->data = data;
    ptr->left_child = NULL;
    ptr->right_child = NULL;
    return ptr;
}

void recursive_inorder(tree_pointer ptr) {
    if (ptr) {
        recursive_inorder(ptr->left_child);
        printf("%c ", ptr->data);
        recursive_inorder(ptr->right_child);
    }
}

void iter_inorder(tree_pointer ptr) {
    int top = -1;
    tree_pointer stack[MAX_STACK_SIZE];
    for (;;) {
        for (; ptr; ptr = ptr->left_child) stack[++top] = ptr;
        if (top < 0) break;
        ptr = stack[top--];
        printf("%c ", ptr->data);
        ptr = ptr->right_child;
    }
}

void iter_preorder(tree_pointer ptr) {
    /* TODO: you must complete this function */
    int top = -1;
    tree_pointer stack[MAX_STACK_SIZE];
    for (;;) {
        for (; ptr; ptr = ptr->left_child) {
            stack[++top] = ptr;
            printf("%c ", ptr->data);
        }
        if (top < 0) break;
        ptr = stack[top--];
        ptr = ptr->right_child;
    }
}

void iter_postorder(tree_pointer ptr) {
    int top = -1;
    tree_pointer stack[MAX_STACK_SIZE];
    tree_pointer last_visited = NULL;
    for (;;) {
        for (; ptr; ptr = ptr->left_child) stack[++top] = ptr;
        if (top < 0) break;
        ptr = stack[top];
        if (ptr->right_child == NULL || ptr->right_child == last_visited) {
            printf("%c ", ptr->data);
            last_visited = ptr;
            top--;
            ptr = NULL;
        } else {
            ptr = ptr->right_child;
        }
    }
}

void level_order(tree_pointer ptr) {
    /* TODO: you must complete this function */
    int front = 0, rear = 0;
    if (!ptr) return; /* empty tree */
    addq(front, &rear, ptr);
    for (;;) {
        ptr = deleteq(&front, rear);
        if (ptr) {
            printf("%c ", ptr->data);
            if (ptr->left_child) addq(front, &rear, ptr->left_child);
            if (ptr->right_child) addq(front, &rear, ptr->right_child);
        } else
            break;
    }
}

/* TODO: add function implementations here */
void addq(int front, int *rear, tree_pointer ptr) {
    *rear = (*rear + 1) % MAX_QUEUE_SIZE;
    queue[*rear] = ptr;
}

tree_pointer deleteq(int *front, int rear) {
    *front = (*front + 1) % MAX_QUEUE_SIZE;
    return queue[*front];
}
