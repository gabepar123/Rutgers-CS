#include <stdio.h>
#include <stdlib.h>

//***********************************************//
//      RETURNS THE VALUE OF nth TO THE NUMBER x //
//***********************************************//
unsigned int get(unsigned int x, int n){

	return (x&(1<<n))!=0; 
}

int main(int argc, char* argv[]){
	
	unsigned short x = atoi(argv[1]);

	int isPalindrome = 1;

	for (int i = 0; i < 8; i++){
		if (get(x, i) != get(x, 15-i)){
			isPalindrome = 0;
			break;
		}
	}
	
	if (isPalindrome == 1){
		printf("Is-Palindrome\n");
	}
	else {
		printf("Not-Palindrome\n");
	}

}
