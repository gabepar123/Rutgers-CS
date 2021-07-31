/*
    wfd_REPO is a 2D linked list that contains each file, and a struct node that contains each word in that file and its frequency
    and a pointer to the next file
*/

typedef struct wfd_repo {
    char* filePath;
    int filePath_length;
    int total_word_count;
    struct wfd_repo* next;
    //Words points to the list that contains each word in a file and its frequency
    struct node* word_list;
} wfd_repo;

//lock for wfd inserts
pthread_mutex_t wfd_lock = PTHREAD_MUTEX_INITIALIZER;

wfd_repo* wfd_repo_insert(wfd_repo* head, char* filePath, int file_length, int total_word_count, node* word_list){

    pthread_mutex_lock(&wfd_lock);
    

    wfd_repo* newNode = malloc(sizeof(wfd_repo));
    mallocErr(newNode);
    //file length INCLUDES NULL-byte
    newNode->filePath = malloc(sizeof(char) * file_length);
    mallocErr(newNode->filePath);
    newNode->filePath_length =  file_length;
    strcpy(newNode->filePath, filePath);
    newNode->total_word_count = total_word_count;
    newNode->next = NULL;
    newNode->word_list = word_list;

    
    if (head == NULL){
        pthread_mutex_unlock(&wfd_lock);
        return newNode;
    }
    newNode->next = head;
    head = newNode;
    
    pthread_mutex_unlock(&wfd_lock);

    return head;

}

void freewfd_repo(wfd_repo* head){
    wfd_repo* ptr = head;
    pthread_mutex_destroy(&wfd_lock);

    while (head != NULL){
        ptr = head;
        head = head->next;
        free(ptr->filePath);
        freeList(ptr->word_list);
        free(ptr);
    }


}


void print_wfd_repo(wfd_repo* head){
    wfd_repo* ptr = head;
    while (ptr != NULL){
        printf("%s\n word-count: %d", ptr->filePath, ptr->total_word_count);
        printList(ptr->word_list);
        printf("\n");
        ptr = ptr->next;
    }
    printf("\n");
}