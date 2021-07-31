/*
Thread safe Variable length Directory Queue
*/

//Directory Queue is VARIABLE length, size doubles when full
//Directory Queue Struct
#define DIR_QSIZE 5


typedef struct {
    int size;
    int active_threads;
	char** data;
	unsigned count;
	unsigned head;
	pthread_mutex_t lock;
	pthread_cond_t read_ready;
} dir_queue;

int dir_init(dir_queue *Q, int threads){

	Q->size = DIR_QSIZE;
    Q->active_threads = threads;
    Q->data = malloc(sizeof(char*) * DIR_QSIZE);
	mallocErr(Q->data);
	Q->count = 0;
	Q->head = 0;
	pthread_mutex_init(&Q->lock, NULL);
	pthread_cond_init(&Q->read_ready, NULL);
	
	return 0;
}

int dir_destroy(dir_queue *Q) {

    free(Q->data);
	pthread_mutex_destroy(&Q->lock);
	pthread_cond_destroy(&Q->read_ready);

	return 0;
}
//path_length includes NULL byte
int dir_enqueue(dir_queue *Q, char* path, int path_length){

	lock(&Q->lock);
	
    //if Queue is full, expand it
	if (Q->count == Q->size) {
        Q->size = Q->size * 2;
        char** p = realloc(Q->data, sizeof(char*) * Q->size);
        if (!p) return 1;
        Q->data = p;
	}
    
	unsigned i = Q->head + Q->count;
	if (i >= Q->size) i -= Q->size;
	
    Q->data[i] = malloc(sizeof(char) * path_length);
	mallocErr(Q->data[i]);
	memcpy(Q->data[i], path, path_length);
	++Q->count;
	
	pthread_cond_signal(&Q->read_ready);
	
	pthread_mutex_unlock(&Q->lock);
	
	return 0;
}

//pass in the file queue so we can close it when needed
char* dir_dequeue(dir_queue *Q, file_queue *F){

	pthread_mutex_lock(&Q->lock);
    
    if (Q->count == 0){
        Q->active_threads--;

        if (Q->active_threads <= 0){
			pthread_mutex_unlock(&Q->lock);
            pthread_cond_broadcast(&Q->read_ready);
			file_qclose(F);
            return NULL;
        }
		//end signals to terminate all other threads
		while (Q->count == 0 && Q->active_threads > 0){
			pthread_cond_wait(&Q->read_ready, &Q->lock);
		}
		if (Q->count == 0){
			pthread_mutex_unlock(&Q->lock);
        	return NULL;
		}
		Q->active_threads++;
    }

    char* temp;
    temp = Q->data[Q->head];
    
    --Q->count;
	++Q->head;
	if (Q->head == Q->size) Q->head = 0;
	
	pthread_mutex_unlock(&Q->lock);
	
	return temp;
}
