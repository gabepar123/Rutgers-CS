#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct Node {
	int data;
	struct Node* next;
};

//const char insert = 'i';
//const char delete = 'd';

struct Node* insert(struct Node* head, int input){
	struct Node* node = malloc(sizeof(struct Node));
	node->data = input;
	node->next = NULL;
	if (head == NULL){
//		printf("null\n");
		head = node;
		return head;
	//	printf("its null\n");
	}
	else {
		struct Node* ptr = head;
		while (ptr != NULL) {
			if (node->data < head->data){
				node->next = head;
				head = node;
				return head;
			}
			else if (ptr->next == NULL){
				ptr->next = node;
				return head;
			}
			else if (node->data < ptr->next->data){
				struct Node* temp = ptr->next;
				ptr->next = node;
				node->next = temp;
				return head;
			}
			ptr = ptr->next;
		}
	}
	
	return head;
}
struct Node* delete( struct Node* head, int input){
	if (head == NULL) return head;
	struct Node* temp;
	if (head->data == input){
		temp = head;
		head = head->next;
		free(temp);
		return head;
	}
	struct Node* ptr = head;
	while (ptr != NULL){
		if ( ptr->next != NULL && ptr->next->data == input){
			if (ptr->next->next == NULL){
				temp = ptr->next;
				ptr->next = NULL;
				free(temp);
				return head;
			}
			temp = ptr->next;
			ptr->next = ptr->next->next;
			free(temp);
			return head;
		}
		ptr = ptr->next;
	
	}
	return head;
}
void printList(struct Node* head){
	struct Node* ptr = head;
	int count = 0;
	while (ptr != NULL){
		count++;
		ptr = ptr->next;
	}
	printf("%d\n",count);
	if (count == 0) return;
	
	
	int check = INT_MAX;
	struct Node* temp;
	while (head != NULL){
		if (check != head->data)
			printf("%d\t", head->data);
		temp = head;
		head = head->next;
		check = temp->data;
		free(temp);
	}
	printf("\n");
	return;
}
int main(int argc, char* argv[]) {
	struct Node* head = NULL;

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
		if (action == 'i'){
			head = insert(head, input);
		}
		else{
			head = delete(head, input);
//			printf("deleting\n");
		}
		
	}
	fclose(fp);
	printList(head);	
	return 0;
}
