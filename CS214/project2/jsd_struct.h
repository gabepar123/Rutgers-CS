/*
	Struct to hold the JSD of 2 files and their names
*/
typedef struct {
	char* file1;
	char* file2;
	double JSD;
	int length;
	int word_count;
} jsd_struct;

//final output of program
void printfJSD(jsd_struct* arr, int comparisons){
    for (int i = 0; i < comparisons; i++){
        printf("%f %s %s\n", arr[i].JSD, arr[i].file1, arr[i].file2);
    }

}

void freeJSD_struct(jsd_struct* arr,  int comparisons){
    for (int i = 0; i < comparisons; i++){
        free(arr[i].file1);
        free(arr[i].file2);
    }
}