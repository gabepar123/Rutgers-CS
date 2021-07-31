/*
FILE QUEUE IS BOUNDED, functions are similar to directory queue
*/
#define FILE_QSIZE 8
pthread_mutex_t file_lock = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int active_threads;
	char** data;
	unsigned count;
	unsigned head;
    int open;
	pthread_mutex_t lock;
	pthread_cond_t read_ready;
    pthread_cond_t write_ready;
} file_queue;


int file_init(file_queue *F, int threads){

    F->active_threads = threads;
    F->data = malloc(sizeof(char*) * FILE_QSIZE);
	mallocErr(F->data);
	F->count = 0;
	F->head = 0;
    F->open = 1;
	pthread_mutex_init(&F->lock, NULL);
	pthread_cond_init(&F->read_ready, NULL);
    pthread_cond_init(&F->write_ready, NULL);
	
	return 0;
}

int file_destroy(file_queue *F){

    free(F->data);
	pthread_mutex_destroy(&F->lock);
	pthread_cond_destroy(&F->read_ready);
	pthread_cond_destroy(&F->write_ready);

	return 0;
}

int file_enqueue(file_queue *F, char* path, int path_length)
{
	pthread_mutex_lock(&F->lock);
	
	while (F->count == FILE_QSIZE && F->open) {
		pthread_cond_wait(&F->write_ready, &F->lock);
	}
	
	unsigned i = F->head + F->count;
	if (i >= FILE_QSIZE) i -= FILE_QSIZE;
	
    //PATH LENGTH includes NULL byte
    F->data[i] = malloc(sizeof(char) * path_length);
	mallocErr(F->data[i]);
	memcpy(F->data[i], path, path_length);
	++F->count;
	
	pthread_cond_signal(&F->read_ready);
	
	pthread_mutex_unlock(&F->lock);
	
	return 0;
}

char* file_dequeue(file_queue *F){

	pthread_mutex_lock(&F->lock);

    while (F->count == 0 && F->open) {
		pthread_cond_wait(&F->read_ready, &F->lock);
	}

    if (F->count == 0) {
		pthread_mutex_unlock(&F->lock);
		return NULL;
	}

    char* temp;
    temp = F->data[F->head];
    
	num_of_files++;
    --F->count;
	++F->head;
	if (F->head == FILE_QSIZE) F->head = 0;

    pthread_cond_signal(&F->write_ready);
	
	pthread_mutex_unlock(&F->lock);
	
	return temp;
}

//Close signals that there will be no more files added
int file_qclose(file_queue *F)
{
	pthread_mutex_lock(&F->lock);
	F->open = 0;
	pthread_cond_broadcast(&F->read_ready);
	pthread_cond_broadcast(&F->write_ready);
	pthread_mutex_unlock(&F->lock);	

	return 0;
}