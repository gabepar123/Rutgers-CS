#include <stdio.h>
#include <string.h>

//***********************************************//
//	RETURNS THE VALUE OF nth TO THE NUMBER x //
//***********************************************//
unsigned int get(unsigned int x, int n){

	return (x&(1<<n))!=0;

}

//************************************************//
//	SETS THE nth BIT OF THE NUMBER x TO v     //
//************************************************//
unsigned int set(unsigned int x, int n, int v){
	
	if (v == 1){
		return x|(1<<n);
	}
	else 
		return x&~(1<<n);
}
//******************************************************//
// SETS THE VALUE OF THE nth BIT OF x to its complement	//
//******************************************************//
unsigned int comp(unsigned int x, int n){
	
	return x^(1<<n);

}

int main(int argc, char* argv[])
{
	FILE* fp = fopen(argv[1], "r");

	unsigned int x;
	fscanf(fp, "%u\n", &x);

	int n;
	int v;
	char command[5];

	while(1){
		int z = fscanf(fp, "%s\t%d\t%d\n", command, &n, &v);		
		if (z != 3)
			break;

		if (command[0] == 'g') {
			printf("%d\n", get(x,n));
		}
		else if (command[0] == 's') {
			x = set(x, n, v);
			printf("%u\n", x);
		}
		else if (command[0] == 'c') {
			x = comp(x,n);
			printf("%u\n", x);
		}

	}



}
