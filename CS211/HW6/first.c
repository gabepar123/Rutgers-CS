#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

struct Node {
	int valid;
	unsigned long long tag;
	int age;
};

//counts
int memoryReads = 0;
int memoryWrites = 0;
int cacheHits = 0;
int cacheMisses = 0;

struct Node** updatePolicy(struct Node** cache, char* replacePolicy, int linesPerSet, int setIndex, int lineIndex){

//fifo
//only when inserting new
	if (strcmp(replacePolicy, "fifo") == 0){
		for (int i = 0; i < linesPerSet; i++){
			if (cache[setIndex][i].valid == 1){
				cache[setIndex][i].age++;
			}
		}
	}
	
//lru
	else if (strcmp(replacePolicy, "lru") == 0){
		for (int i = 0; i < linesPerSet; i++){
			if (cache[setIndex][i].valid == 1){
				cache[setIndex][i].age--;
			}
		}
		cache[setIndex][lineIndex].age = 0;
	}


	return cache;
}

//REPLACE DATA USING POLICY
int replace(struct Node** cache, char* replacePolicy, int setIndex, int linesPerSet){

//fifo
//find max and return it to be replaced

	if (strcmp(replacePolicy, "fifo") == 0){
		int maxIndex = 0;
		for (int i = 1; i < linesPerSet; i++){
			if (cache[setIndex][i].valid != 1){
				printf("REPLACE POLICY IN NONFULL CACHE\n");
			}
			if (cache[setIndex][i].age > cache[setIndex][maxIndex].age){
				maxIndex = i;
			}
		}
		return maxIndex;
	}
//lru
//find min and return it
	else if (strcmp(replacePolicy, "lru") == 0){
		int minIndex = 0;
		for (int i = 1; i < linesPerSet; i++){
			if (cache[setIndex][i].valid != 1){
				printf("REPLACE PLOCYU IN NONFUL CACHE\n");
			}
			if (cache[setIndex][i].age < cache[setIndex][minIndex].age){
				minIndex = i;
			}
		}
		return minIndex;
	}
	printf("replace error\n");
	return -1;

}

struct Node** search(struct Node** cache, int numOfSets, int linesPerSet, unsigned long long setIndex, unsigned long long tag, char op, char* replacePolicy){

	if (op == 'W'){
		memoryWrites++;
	}

	//find a matching tag
	for (int i = 0; i < linesPerSet; i++){
		if (cache[setIndex][i].tag == tag && cache[setIndex][i].valid == 1){
			//HIT
			cacheHits++;
			if (strcmp(replacePolicy, "lru") == 0){
				cache =	updatePolicy(cache, replacePolicy, linesPerSet, setIndex, i);
			}
			return cache;
		}
	}

	memoryReads++;
	cacheMisses++;
	//IF MISS
	for (int i  = 0; i < linesPerSet; i++){
		if (cache[setIndex][i].valid == -1){
			cache[setIndex][i].valid = 1;
			cache[setIndex][i].tag = tag;
			cache = updatePolicy(cache, replacePolicy, linesPerSet, setIndex, i);
			return cache;
		}
	}

	//IF MISS AND FULL
	//REPLACEMENT
	int replaceIndex = replace(cache, replacePolicy, setIndex, linesPerSet);
			cache[setIndex][replaceIndex].age = 0;
			cache[setIndex][replaceIndex].tag = tag;
			cache = updatePolicy(cache, replacePolicy, linesPerSet, setIndex, replaceIndex);
			
	return cache;
	
}

//PRINT CACHE
void printCache(struct Node** cache, int numOfSets, int linesPerSet){
	for (int i = 0; i < numOfSets; i++){
		for (int j = 0; j < linesPerSet; j++){
			printf("Valid: %d Tag: %llx Count: %d\t", cache[i][j].valid, cache[i][j].tag, cache[i][j].age);  
		
		}
		printf("\n");
	}
}

//FREE THE CACHE
void freeCache(struct Node** cache, int numOfSets, int linesPerSet){

	for (int i = 0; i < numOfSets; i++){
		//for (int j = 0; j < linesPerSet; j++){
			free(cache[i]);
		}
	//}
	free(cache);
}

//checks if args are powers of 2 
//declaration
bool isPowerOfTwo(int x);
//error check args
bool hasError(int argc, char* argv[]){

 	if (argc != 6){
                printf("error\n");
                return true;
        }

        if (strcmp(argv[3], "lru") != 0 && strcmp(argv[3], "fifo") != 0){
                printf("error\n");
                return true;
        }

        if (!isPowerOfTwo(atoi(argv[1])) || !isPowerOfTwo(atoi(argv[4]))){
                printf("error\n");
                return true;
        }

        FILE *fp = fopen(argv[5], "r");

        if (!fp){
                printf("error\n");
                return true;
        }

        fclose(fp);
	
	 if (strcmp(argv[2], "direct") != 0 && strstr(argv[2], "assoc") == NULL){
                printf("error assoc\n");
		return true;
        }

	return false;
}
//checks if args are power of two
bool isPowerOfTwo(int x){

	return (x != 0) && ((x & (x - 1)) == 0);

}
/*
argv[1] = cache size
argv[2] = associativity
argv[3] = replacement policy
argv[4] = block size
argv[5] = trace file
*/
int main(int argc, char* argv[]){
	//error checking
	if (hasError(argc, argv)){
		return 1;
	}
	int cacheSize = atoi(argv[1]);
//	char* assocWay; //initialized later
	char* replacePolicy = argv[3];
	int blockSize = atoi(argv[4]);

	int numOfSets;
	int linesPerSet;
	
	//direct cache
	if (strcmp(argv[2], "direct") == 0){
//		assocWay = "direct";
		//SETTING LINES AND SET
		linesPerSet = 1;
		numOfSets = cacheSize / blockSize;
	}
	// n-way cache
	else if (strchr(argv[2], ':') != NULL){
//		assocWay = "assoc";
		char* token = strtok(argv[2], ":");
		token = strtok(NULL, ":");
		//more error checking
		if (token == NULL){
			printf("error\n");
			return 1;
		}
		if (isdigit(token[0])){
			//SETTING LINES AND SET
			linesPerSet = atoi(token);
			numOfSets = cacheSize / (linesPerSet * blockSize);
			if (!isPowerOfTwo(linesPerSet)){
				printf("error\n");
				return 1;
			}
		}
		else {
			printf("error\n");
			return 1;
		}
	}
	//fully assoc cache
	else {
//		assocWay = "assoc";
		linesPerSet = cacheSize / blockSize;
		numOfSets = 1;
	}
	//create Cache
	struct Node** cache = malloc(numOfSets * sizeof(struct Node*));
	for (int i = 0; i < numOfSets; i++){
		cache[i] = malloc(linesPerSet * sizeof(struct Node));
		for (int j = 0; j < linesPerSet; j ++){
		
			cache[i][j].valid  = -1;
			cache[i][j].tag = 0;
			cache[i][j].age = -1;
		}

	}
	
	//Find set and tag index bits
	int blockIndexBits = log(blockSize)/log(2); //length
	int setIndexBits = log(numOfSets)/log(2);  //length
	int tagIndexBits = 48 - blockIndexBits - setIndexBits;  //length
	
	unsigned long long setIndex;
	unsigned long long tag;
	char op;
	unsigned long long address; 

	FILE* file = fopen(argv[5], "r");
	
	while(fscanf(file, "%*s %c %llx\n", &op, &address) != EOF){
//		printf("%llu\n", address);
//		blockOffset = address & ((long long)pow(2,(blockIndexBits)) - 1);   //actual bits
        	setIndex = (address>> blockIndexBits) & ((unsigned long long)pow(2,(setIndexBits)) - 1);    //actual bits
        	tag = (address>>(blockIndexBits + setIndexBits)) & ((unsigned long long)pow(2, (tagIndexBits)) - 1); //actual bits
		search(cache, numOfSets, linesPerSet, setIndex, tag, op, replacePolicy);
	}
	fclose(file);
/*	
	printf("linesPerSet: %d\n", linesPerSet);
        printf("assocWay: %s\n", assocWay);
        printf("numOfSets: %d\n", numOfSets);
        printf("setIndexBits Length : %d\n", setIndexBits);
        printf("tagIndexBits Length: %d\n", tagIndexBits);
        printf("setIndex Actual Bits: %llx\n", setIndex);
        printf("tag Actual Bits: %llx\n", tag);
	printf("\n");
*/
	printf("Memory reads: %d\n", memoryReads);
	printf("Memory writes: %d\n", memoryWrites);
	printf("Cache hits: %d\n", cacheHits);
	printf("Cache misses: %d\n", cacheMisses);
//	printCache(cache, numOfSets, linesPerSet);
	freeCache(cache, numOfSets, linesPerSet);
}
