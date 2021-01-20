#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct var{
	char name[20];
	int bit;
	struct var* next;
};

struct var** createTruthTable(struct var** truthTable, int numInputVars, int numCombinationsi, char** tempInputVarArray, char** tempOutputVarArray, int numOutputVars);
void freeTruthTable(struct var** truthTable, int numInputVars, int numCombinations);
void printTruthTable(struct var** truthTable, int numInputVars, int numCombinations, int numOutputVars);
void getVarName(char op[], char xString[], char yString[], char outString[], char line[]);
void freeArray(char** arr, int length);
/*Operators*/
int not(int x);
int and(int x, int y);
int or(int x, int y);
int nand(int x, int y);
int nor(int x, int y);
int xor(int x, int y);

int not(int x){
	if (x)
		return 0;
	return 1;
}
int and(int x, int y){
	return x * y;
}
int or(int x, int y){
	if (x + y >= 1)
		return 1;
	return 0;
}
int nand(int x, int y){
	return not(and(x,y));
}
int nor(int x, int y){
	return not(or(x,y));
}
int xor(int x, int y){
	if (x+y==1)
		return 1;
	return 0;
}

/* Creates Truth Table, duh */
struct var** createTruthTable(struct var** truthTable, int numInputVars, int numCombinations, char** tempInputVarArray, char** tempOutputVarArray, int numOutputVars){
	
	for (int i = 0; i < numCombinations; i++){
		int k = 0;
		for (int j = 1 << (numInputVars-1); j > 0; j = j/2){
			if (i & j)
				truthTable[i][k].bit = 1;
			else
				truthTable[i][k].bit = 0;
		//	truthTable[i][k].name = "i";
			strcpy(truthTable[i][k].name, tempInputVarArray[k]);
			k++;
		}
		int l = 0;
		for (int o = numInputVars; o < numInputVars + numOutputVars; o++){
			truthTable[i][o].bit = -1;
			strcpy(truthTable[i][o].name, tempOutputVarArray[l]);
			l++;
		}
		
	}
	return truthTable;
}
/* Frees Truth Table */
void freeTruthTable(struct var** truthTable, int numInputVars, int numCombinations){
	
	for (int i = 0; i < numCombinations; i++){
		free(truthTable[i]);
	}
	
	free(truthTable);
}
/*Prints Truth Table */
void printTruthTable(struct var** truthTable, int numInputVars, int numCombinations, int numOutputVars){
	for (int i = 0; i < numCombinations; i++){
		for (int j = 0; j < numInputVars + numOutputVars; j++){
			printf("%d ", truthTable[i][j].bit); 
	//		printf("%s ", truthTable[i][j].name);
		
		}
		printf("\n");
	}


}

void freeArray(char** arr, int length){
	
	for (int i = 0; i < length; i++){
		free(arr[i]);
	}
	free(arr);
}
/* Gets Var names for both inputs and outputs */
void getVarName(char op[], char xString[], char yString[], char outString[], char line[]){
	
	char* token = strtok(line, " \n");
	strcpy(op, token);
	token = strtok(NULL, " \n");
	strcpy(xString, token);
	token = strtok(NULL, " \n");
	strcpy(yString, token);
	if (strcmp(op, "NOT") == 0){
		return;
	}
	token = strtok(NULL, " \n");
	strcpy(outString, token);

}
/* Finds index value for inputs in array, if it exists */
int getIndexValue(struct var** truthTable, int numInputVars, int numOutputVars, int row, char name[]){
	
	for (int i = 0; i < numInputVars; i++){
		if (strcmp (truthTable[row][i].name, name) == 0){
			return truthTable[row][i].bit;
		}	
//		printf("truth: %s name: %s\n", truthTable[row][i].name, name);	
	}	
	return -1;
}
/* Gets temp variable value for input */
int getTempVar(char name[], struct var* head){

//	int tempVal;
	struct var* ptr = head;

	while (ptr != NULL){
	//	printf("HERE\n");
		if (strcmp(ptr->name, name) == 0){
			return ptr->bit;
		}	
		ptr = ptr->next;
	}
	return -1;
} 
/* Changes output var value in truthtable or in LL */
struct var* getOutputVar(struct var** truthTable, char name[], int row, int numInputVars, int numOutputVars, int output, struct var* head){
	//check array for output var
	for (int i = numInputVars; i < numInputVars + numOutputVars; i++){
		if (strcmp(truthTable[row][i].name, name) == 0){
			truthTable[row][i].bit = output;
			return head;
		}
	}
	//create temp outputs
	struct var* temp = malloc(sizeof(struct var));
        strcpy(temp->name, name);
        temp->bit = output;
        temp->next = NULL;
	
	if (head == NULL){
		head = temp;
	}
	else {
		temp->next = head;
		//head->next = temp;
	}
	return temp;
}
/* does bit Op on X and Y */
int doOP(int x, int y, char op[]){
	//DOES NOT INCLUDE NOT 
	if (strcmp(op, "AND") == 0)
		return and(x,y);

	else if (strcmp(op, "OR") == 0)
		return or(x,y);

	else if (strcmp(op, "NAND") == 0)
		return nand(x,y);

	else if (strcmp(op, "NOR") == 0)
		return nor(x,y);

	else if (strcmp(op, "XOR") == 0)
		return xor(x,y);
	else if (strcmp(op, "NOT") == 0)
		return not(x);
	
	return -1;
}
void freeLL(struct var* head){
	struct var* ptr;

	while (head != NULL){
		ptr = head;
		head = head->next;
		free(ptr);
	}

}
int main(int argc, char* argv[]){
	/* Initialize # of input and output*/
	int numInputVars;
	int numOutputVars;

	/* Gets # of input and output vars */
	FILE* fpGrabber = fopen(argv[1], "r");
	fscanf(fpGrabber, "%*s %d %*[^\n]", &numInputVars);
	fscanf(fpGrabber, "%*s %d ", &numOutputVars);
	fclose(fpGrabber);

	/* 2^n */
	int numCombinations = (int)pow(2, numInputVars);
	
	/* Fills in Truth Table with Variable Names */
	char** tempInputVarArray = malloc(numInputVars * sizeof(char*));
	char** tempOutputVarArray = malloc(numOutputVars * sizeof(char*));
	
	FILE* fpVarNames = fopen(argv[1], "r");
	char temp[100];
	fgets(temp, 100, fpVarNames);
	char* token = strtok(temp, " \n");
	token = strtok(NULL, " \n");
	token = strtok(NULL, " \n");

	for (int i = 0; i < numInputVars; i++){
		
		tempInputVarArray[i] = malloc(20);
		strcpy(tempInputVarArray[i], token);
		token = strtok(NULL, " \n");
	}
	fgets(temp, 100, fpVarNames);
	char* token_2 = strtok(temp, " \n");
	token_2 = strtok(NULL, " \n");
	token_2 = strtok(NULL, " \n");
	for (int i = 0; i < numOutputVars; i++){
                
		tempOutputVarArray[i] = malloc(20);
		strcpy(tempOutputVarArray[i], token_2);
                token_2 = strtok(NULL, " \n");
	}
	fclose(fpVarNames);
	/* */
	
	/* Initalize Truth Table */
        struct var** truthTable = malloc(numCombinations * sizeof(struct var*));

        for (int i = 0; i < numCombinations; i++){
                truthTable[i] = malloc((numInputVars + numOutputVars) * sizeof(struct var));
        }
        truthTable = createTruthTable(truthTable, numInputVars, numCombinations, tempInputVarArray, tempOutputVarArray, numOutputVars);
        /* */


//	struct var* head = NULL;


	/* Main for loop */
	for (int i = 0; i < numCombinations; i++){
	 	struct var* head = NULL;	
		FILE* fp = fopen(argv[1], "r");
		char line[100];
		fgets(line, 100, fp);
		fgets(line, 100, fp);
		
		char op[6] = "hi";
		char xString[20];
		char yString[20];
		char outString[20];

		while(fgets(line, 100, fp) != NULL){
			getVarName(op, xString, yString, outString, line);
			int x = getIndexValue(truthTable, numInputVars, numOutputVars, i, xString);					
			if (x == -1){
				x = getTempVar(xString, head);
			//	 printf("index %d xString: %s x: %d \n", i, xString, x);
					/*if (x == -1){
						printf("index = %d X ERROR\n", i);
					 }*/
			}
			if (strcmp(op, "NOT") == 0){
				 int output = doOP(x,-1,op);
				 head = getOutputVar(truthTable, yString, i, numInputVars, numOutputVars, output, head);
				 continue;
			}
			int y = getIndexValue(truthTable, numInputVars, numOutputVars, i, yString);
			if (y == -1){
				y = getTempVar(yString, head);
					/*if (y == -1){
						printf("index = %d Y Error\n", i);
					}*/
			}	
			int output = doOP(x, y, op);
			head = getOutputVar(truthTable, outString, i, numInputVars, numOutputVars, output, head);
		}

		fclose(fp);
		freeLL(head);
	} 
		
	
	
		

	

	/* Initalize Truth Table
	struct var** truthTable = malloc(numCombinations * sizeof(struct var*));

        for (int i = 0; i < numCombinations; i++){
                truthTable[i] = malloc((numInputVars + numOutputVars) * sizeof(struct var));
        }
	truthTable = createTruthTable(truthTable, numInputVars, numCombinations, tempInputVarArray, tempOutputVarArray, numOutputVars);
	*/
	
	/* Output */
	printTruthTable(truthTable, numInputVars, numCombinations, numOutputVars);

	/* Free Everything */
	freeTruthTable(truthTable, numInputVars, numCombinations);
	freeArray(tempInputVarArray, numInputVars);
	freeArray(tempOutputVarArray,  numOutputVars);
}
