#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
struct Node{
	int data;
	struct Node* next;

};

int n = 10000;
int collisions = 0;
int searches = 0;

//*******************KEY***************//
int hashFunction(int input){
	int function = input % n;
	if (function < 0)
		function = function + n;
	return function;
}

void insert(struct Node** table, int input){
	int function = hashFunction(input);
	struct Node* node = malloc(sizeof(struct Node));
	node->data = input;
	node->next = NULL;	
	struct Node* ptr = table[function];
	//printf("%zu\n", ptr);

	if (ptr == NULL){
//		printf("NULL bro\n");
		table[function] = node;
		return;
	}
	else {
//		printf("welp\n");
		collisions++;
		while (ptr != NULL){
                	if (ptr->data == input){
           			free(node);
				return;  
                  	}
			else if (ptr->next == NULL){
				ptr->next = node;
				return;
			}
         		ptr = ptr->next;
		} 
		
	}
	return;
}

//1 if call is from main
void search(struct Node** table, int input){
	int function = hashFunction(input);

	struct Node* ptr = table[function];
	while (ptr != NULL){
		if (ptr->data == input){
			searches++;
			break;
		}
		ptr = ptr->next;	
	}

}
void freeTable(struct Node** table){
//	free(table);
		for (int i = 0; i < n; i++){
			struct Node* ptr = table[i];
			while (ptr != NULL){
				struct Node* temp = ptr;
				//free(ptr);
				ptr = ptr->next;
				free(temp);
			}
		}

/*	for(int i = 0; i < n; i++){
		free(table[i]);
	}
*/
	//free(table);	
		
	
}
int main(int argc, char* argv[])
{

	struct Node* table[n];  //malloc(sizeof(struct Node)*n);
	for (int i = 0; i < n; i++){
		//table[i] = malloc(sizeof(struct Node));
		table[i] = NULL;
	
	}

	FILE* fp = fopen(argv[1], "r");

         while (1) {
	 	int input;
	 	char action;
	 	int n = fscanf(fp, "%c\t%d\n", &action, &input);
		if (n != 2) 
			break;
		if (action == 'i')
			insert(table, input);
		else 
			search(table, input);
	}
	printf("%d\n", collisions);
	printf("%d\n", searches);
	freeTable(table);
//	free(table);
}
