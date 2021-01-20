#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char* argv[]) {
	char* vowels = "aeiouAEIOU";	
	char output[1000] = "";

	for (int i = 1; i < argc; i++){
		for (int j = 0; j < strlen(argv[i]); j++){
			for (int k = 0; k < strlen(vowels); k++){
				if (argv[i][j] == vowels[k]){
					strncat(output, &vowels[k], 1);	
				}
			
			
			}
		
		}
	}
	printf("%s\n", output);
	//free(output);




}
