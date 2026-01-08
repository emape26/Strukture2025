#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 10

typedef struct Node {
	int value;
	struct Node *left;
	struct Node* right;
}Node;

void printTree(Node* root, int level);
Node* createNode(int value);
Node* insert(Node* root, int x);
int replace(Node* root);
void ToFile(Node* root, FILE* f);
void freeTree(Node* root);

int main() {

	srand((unsigned)time(NULL));

	int a[N];

	printf("Generirani Brojevi: ");
	for (int i = 0; i < N; i++) {
		a[i] = (rand() % 81) + 10;
		printf("%d ", a[i]);
	}
	printf("\n");

	Node* root = NULL;

	for (int i = 0;i < N;i++) {
		root = insert(root, a[i]);
	}

	FILE* f = fopen("inorder.txt", "w");
	if (f == NULL) {
		printf("Greska");
		freeTree(root);
		return 1;
	}
	printf("\nStablo nakon (a) - insert:\n");
	printTree(root, 0);

	fprintf(f, "INORDER nakon (a) - nakon insert:\n");
	ToFile(root, f);
	fprintf(f, "\n\n");


	replace(root);

	printf("\nStablo nakon (b) - replace:\n");
	printTree(root, 0);

	fprintf(f, "INORDER nakon (b) - nakon replace:\n");
	ToFile(root, f);
	fprintf(f, "\n");

	fclose(f);
	printf("\nUpisano u datoteku: inorder.txt\n");

	freeTree(root);
	root = NULL;

	return 0;
}


Node* createNode(int value) {
	Node* new = (Node*)malloc(sizeof(Node));
	if (new == NULL) {
		printf("Greska u alokaciji");
		return NULL;
	}

	new->value = value;
	new->left = NULL;
	new->right = NULL;
	return new;

}

Node* insert(Node* root, int x) {
	if (root == NULL) {
		return createNode(x);
	}

	if (x >= root->value) {
		root->left = insert(root->left, x);
	}
	else {
		root->right = insert(root->right, x);
	}
	return root;
}

int replace(Node* root) {
	if (root == NULL) {
		return 0;
	}

	int prevValue = root->value;

	int leftSum = replace(root->left);
	int rightSum = replace(root->right);

	root->value = leftSum + rightSum;

	return root->value + prevValue;
}

void ToFile(Node* root, FILE* f) {
	if (root == NULL)return;

	ToFile(root->left, f);
	fprintf(f, "%d ", root->value);
	ToFile(root->right, f);
}

void freeTree(Node* root) {
	if (root == NULL)return;
	freeTree(root->left);
	freeTree(root->right);
	free(root);
}
void printTree(Node* root, int level) {
	if (root == NULL) return;

	printTree(root->right, level + 1);

	for (int i = 0; i < level; i++) printf("    ");
	printf("%d\n", root->value);

	printTree(root->left, level + 1);
}