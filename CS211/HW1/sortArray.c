#include <stdio.h>
#include <stdlib.h>

void swap(int* arr, int indexOne, int indexTwo){
 
         int temp = arr[indexOne];
         arr[indexOne] = arr[indexTwo];
         arr[indexTwo] = temp;
  
  
 }

void sortEven(int* arr, int length){
 
         for (int i = 0; i < length-1; i++){
                 for (int j = 0; j < length-i-1; j++){
                         if (arr[j] > arr[j+1]){
                                 swap(arr, j, j+1);
                         }
                 }
 
 
         } 
 
 }
void sortOdd(int* arr, int length){

          for (int i = 0; i < length-1; i++){
                 for (int j = 0; j < length-i-1; j++){
			if (arr[j] < arr[j+1]){
                                  swap(arr, j, j+1);
 			}
 		}		


           }
  //      printArray(arr,length);
 
 }



void partition(int* arr, int evens, int odds, int length){
	if (evens == 0 || odds == 0) return;
	
	int *evenArr = malloc(sizeof(int)*evens); 
	int *oddArr = malloc(sizeof(int)*odds);
	int evenIndex = 0; 
	int oddIndex = 0;

	for (int i = 0; i < length; i++){
		if (arr[i] % 2 == 0){
			evenArr[evenIndex] = arr[i];
			evenIndex++;
		}
		else {
			oddArr[oddIndex] = arr[i];
			oddIndex++;
		}	
	}

	sortEven(evenArr, evens);
	sortOdd(oddArr, odds);

	for (int i = 0; i < evens; i++){
		arr[i] = evenArr[i];
	}
	oddIndex = 0;
	for (int i = evens; i < length; i++){
		arr[i] = oddArr[oddIndex];
		oddIndex++;
	}


	free(evenArr);
	free(oddArr);
	
}
/*
void swap(int* arr, int indexOne, int indexTwo){
	
	int temp = arr[indexOne];
	arr[indexOne] = arr[indexTwo];
	arr[indexTwo] = temp;
	

}
*/
void printArray(int *arr, int length){
	for (int i = 0; i < length; i++){
		printf("%d\t", arr[i]);
        }
        printf("\n");
}
/*
void sortEven(int* arr, int length){

	for (int i = 0; i < length-1; i++){
		for (int j = 0; j < length-i-1; j++){
			if (arr[j] > arr[j+1]){
				swap(arr, j, j+1);
			}
		}
	
	
	}


}
void sortOdd(int* arr, int length){

         for (int i = 0; i < length-1; i++){
  		for (int j = 0; j < length-i-1; j++){
			printf("swapping\n");
			if (arr[j] < arr[j+1]){
                                 swap(arr, j, j+1);
                         }
                 }


         }
//	printArray(arr,length);

}


void printArray(int *arr, int length){
	for (int i = 0; i < length; i++){
		printf("%d\t", arr[i]);
	}
	printf("\n");
}
*/

int main(int argc, char* argv[]) {
 
	FILE* fp = fopen(argv[1], "r");
 
 	int length;
	fscanf(fp, "%d", &length);
        
	//printf("%d\n", length);

	int *arr = malloc(sizeof(int) * length);
	int odds;
	for (int i = 0; i < length; i++){
                fscanf(fp, "%d\t", &arr[i]);
		if (arr[i] % 2 != 0)
			odds++;
	}
	int evens = length - odds;
	fclose(fp);	
	//printf("evens: %d odds: %d\n", evens,odds);
/*	int *evenArr = malloc(sizeof(int)*evens);
	int *oddArr = malloc(sizeof(int)*odds);
*/
	//printArray(arr, length);
	partition(arr, evens, odds, length);
	if (evens != 0 && odds == 0)
		sortEven(arr,length);
	if (odds != 0 && evens == 0)
		sortOdd(arr,length);
	
	printArray(arr, length);
	free(arr);
	return 0;
}

