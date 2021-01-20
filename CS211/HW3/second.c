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
	
	int pairs = 0;
	int oneBits = 0;
	int prev = -1;
	int skip = -1;
	for (int i = 0; i < 16; i++){
		int curr = get(x,i);
		if (curr == 1){
			oneBits++;
		}
	
		if (prev != -1){
			if (prev == 1 && curr == 1 && skip != i-1){
				pairs++;
				skip = i;
			}
		}
		prev = curr;
	}
	if (oneBits%2==0){
		printf("Even-Parity\t%d\n", pairs);
	}
	else {
		printf("Odd-Parity\t%d\n", pairs);
	
	}

}
