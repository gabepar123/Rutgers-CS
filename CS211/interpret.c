#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//registers
short ax;
short bx;
short cx;
short dx;
//delim 
char delim[] = " ";

int compare(char* ptr, short x, short y){

	switch(ptr[1]){
		
		case 'e':
			return x == y;
		case 'n':
			return x !=y;
		case 'g':
			if (ptr[2] == 'e')
				return x >= y;
			else 
				return x > y;
		case 'l':
			if (ptr[2] == 'e')
				return x <= y;
			else 
				return x < y;
		default:
			printf("compare ERROR\n");
			break;
	}

	printf("compare ERROR RETURN\n");
	return -1;

}



//Math operators/move
int jump(char* ptr, int index){
	//store which jump it is
	char temp[10];
	strcpy(temp, ptr);
	
	//move to line number and store
	ptr = strtok(NULL,delim);


	int line = atoi(ptr);
			
	//if its jmp
	if (temp[1] == 'm'){
		return line;
	}

	short x;
	short y;
	//find x or source
	ptr = strtok(NULL,delim);

	
	if (isdigit(ptr[0]) || isdigit(ptr[1])){
        	x = atoi(ptr);
         }
         else {
                 switch(ptr[0]){
                         case 'a':
                                 x = ax;
                                 break;
                         case 'b':
                                 x = bx;
 				break;
                         case 'c':
                                 x = cx;
                                 break;
                         case 'd':
                                 x = dx;
                                 break;
                         default:
                                 printf("X instruction error\n");
                                 break;
                 }
         }	

	 //find y or destination
	 ptr = strtok(NULL,delim);
	
	 if (isdigit(ptr[0]) || isdigit(ptr[1])){
                 y = atoi(ptr);
          }
          else {
                  switch(ptr[0]){
                          case 'a':
                                  y = ax;
                                  break;
                          case 'b':
                                  y = bx;
                                 break;
                          case 'c':
                                  y = cx;
                                  break;
                          case 'd':
                                  y = dx;
                                 break;
                          default:
                                  printf("Y instruction error\n");
                                  break;
                  }
      	}

	if (compare(temp,x,y))
		return line;
	else 
		return index;


}

short mathMov(char op, short source, short destination){

	switch(op){
		case 'm':
			destination = source;
			break;
		case '+':
			destination = source + destination;
			break;
		case '-':
			destination = destination - source;
			break;
		case '*':
			destination = source * destination;
			break;
		case '/':
			destination = source / destination;
			break;
		default:
		printf("mathMov error\n");
	}
	
	return destination;
}

void instructions(char* ptr){


	char op;
	//find out which instruction to do
	switch(ptr[0]){
		case 'm':
			if (ptr[1] == 'o')
				op = 'm';
			else 
				op = '*';
			break;
		case 'a':
			op = '+';
			break;
		case 's':
			op = '-';
			break;
		case 'd':
			op = '/';
			break;
		default:
			printf("Instructions choice error\n");
			break;
	}
	
	//source
	ptr = strtok(NULL, delim);

	short source;
	if (isdigit(ptr[0]) || isdigit(ptr[1])){
		source = atoi(ptr);		
	}
	else {
		switch(ptr[0]){
			case 'a':
				source = ax;
				break;
			case 'b':
				source = bx;
				break;
			case 'c':
				source = cx;
				break;
			case 'd':
				source = dx;
				break;
			default:
				printf("Source instruction error\n");
				break;
		
		}
	}
	//destination
	ptr = strtok(NULL, delim);
	
	switch(ptr[0]){
		case 'a':
			ax = mathMov(op,source,ax);
			break;
		case 'b':
			bx = mathMov(op,source,bx);
			break;
		case 'c':
			cx = mathMov(op,source,cx);
			break;
		case 'd':
			dx = mathMov(op,source,dx);
			break;
		default:
			printf("source instruction error\n");
			break;
	}


}



int main(int argc, char* argv[])
{
//Loads input into array
	char feed[100][25] = {0};
	FILE *fp = fopen(argv[1], "r");

	for (int i = 0; i < 100; i++){
		feed[i][0] = '\n';
		fgets(feed[i], 25, fp);
//		printf("I = %d, %s", i, feed[i]);
	}
	fclose(fp);

	short temp;
	int line;

//Main for loop
for (int i = 0; i < 100; i++){

	if (strcmp(feed[i],"") != 0){
		//tokenizer
		char tempString[25] = " ";
		strcpy(tempString, feed[i]);
		char* ptr = strtok(tempString, delim);
		//READ
		if (ptr[0] == 'r'){
			ptr = strtok(NULL, delim);
			scanf("%hi", &temp);

			switch (ptr[0]){
				case 'a':
					ax = temp;
					break;
				case 'b':
					bx = temp;
					break;
				case 'c':
					cx = temp;
					break;
				case 'd':
					dx = temp;
					break;
				default:
					printf("OH NO!!!\n");
					break;
			}
		}
		//PRINT
		else if (ptr[0] == 'p'){
			ptr = strtok(NULL, delim);
			
			if (isdigit(ptr[0])){
				printf("%hi", atoi(ptr));
			}
			
			else {
				switch (ptr[0]){
					case 'a':
						printf("%hi", ax);
						break;
					case 'b':
						printf("%hi", bx);
						break;
					case 'c':
						printf("%hi", cx);
						break;
					case 'd':
						printf("%hi", dx);
						break;
					default:
						printf("PRINT ERROR\n");
						break;
				}
			}
		}
	
		else if (ptr[0] == 'j'){
			line = jump(ptr,i);
			if (line == i){
				continue;
			}
			i = line-1;
		}		
	
		else if( ptr[0] == 'm' || ptr[0] == 's' || ptr[0] == 'a'|| ptr[0] == 'd') {
			instructions(ptr);
		}
	}
	
}

}
