#include <stdio.h>
#include <stdlib.h>

struct Node{
	int data;
	struct Node* left;
	struct Node* right;
};

//returns 1 if found
int search(struct Node* root, int input){
	if (root == NULL) return 0;
	if (root->data == input) return 1;

	search(root->left, input);
	search(root->right, input);
	return 0;
}
struct Node* newInput(int input){
	struct Node* node = malloc(sizeof(struct Node));
        node->data = input;
        node->right = NULL;
        node->left = NULL;
	return node;
}
struct Node* insert(struct Node* root, int input){
//	if (search(root,input) == 1) return root; 	
	

	if (root == NULL){
		return newInput(input);
	}
	if (input < root->data){
		root->left = insert(root->left, input);
	}
	else if (input > root->data){ 
		root->right = insert(root->right, input);
	}
	return root;	

}

void printBST(struct Node* root){
	if (root == NULL) return;
	printBST(root->left);
	printf("%d\t", root->data);
	printBST(root->right);
}
void freeBST(struct Node* root){
	if (root == NULL) return;
	freeBST(root->left);
	freeBST(root->right);
	free(root);
}


int main(int argc, char* argv[])
{
	 struct Node* root = NULL;

         FILE* fp = fopen(argv[1], "r");
         if (fp == NULL) {
                 printf("error");
                 return 0;
         } 
         while (1) {
                 int input;
                 char action;
                 int n = fscanf(fp, "%c\t%d\n", &action, &input);
                 if (n != 2)
                         break;
		if (action == 'i')
			root = insert(root, input);
	}
	printBST(root);
	freeBST(root);
}
