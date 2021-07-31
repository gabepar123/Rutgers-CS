#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <errno.h>
#include <math.h>
#include <assert.h>

#define BUFSIZE 10
#define min(a,b) (((a) < (b)) ? (a) : (b))

#define lock(X) \
    do { \
        int err = pthread_mutex_lock(X); \
            if (err) { \
                errno = err; \
                perror("lock"); \
                abort(); \
            } \
    } while (0) \

#define unlock(X) \
    do { \
        int err = pthread_mutex_unlock(X); \
            if (err) { \
                errno = err; \
                perror("unlock"); \
                abort(); \
            } \
    } while (0) \

#define mallocErr(X) \
	do { \
		if (X == NULL) { \
     		perror("malloc"); \
        	exit(EXIT_FAILURE); \
    	} \
	} while (0) \

int num_of_files = 0;

#include "file_queue.h"
#include "dir_queue.h"
#include "strbuf.h"
#include "node.h"
#include "wfd_repo.h"
#include "jsd_struct.h"

int returnError = false;
pthread_mutex_t tokenizer_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t anal_lock = PTHREAD_MUTEX_INITIALIZER;
dir_queue D;
file_queue F;
wfd_repo* head = NULL;
jsd_struct* jsd_array;
int dir_threads = 1, file_threads = 1, anal_threads = 1;
char* fileSuffix;
int comparisons;
int work_load;
wfd_repo* global_ptr1 = NULL;    
wfd_repo* global_ptr2 = NULL;

//Function Delcarations
int isDirectory(char* dirName);
int ignoreFileName(char* file);
char* searchDirectory(char* dirName);
void* directoryThread(void* arg);
void* fileThread(void* arg);
void* analThread(void* arg);
double calculateJSD(node* file1, node* file2);
int isSymbol(char c);
int isDelim(char c);
node* tokenize(char* file);
node* calculateWFD(node* head, int word_count);
void printfJSD(jsd_struct* arr, int comparisons);
void freeJSD_struct(jsd_struct* arr,  int comparisons);
int  compare(const void *a, const void *b);
