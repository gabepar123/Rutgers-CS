/*
    Node contains each word and its frequency for an individual file, and total word count of the file
    Inserts alphabetically
*/
typedef struct node {
    char* word;
    int count;
    int overall_word_count;
    double frequency;
    struct node* next;
} node;

pthread_mutex_t node_lock = PTHREAD_MUTEX_INITIALIZER;
/*
Linked list functions
*/

//INSERTS ALPHABETICALLY
node* insert(node* head, char* word, int word_length){


    node* newNode = malloc(sizeof(node));
    mallocErr(newNode);
    //word length INCLUDES NULL-byte
    newNode->word = malloc(sizeof(char) * word_length);
    mallocErr(newNode->word);
    strcpy(newNode->word,word);
    newNode->count = 1;
    newNode->frequency = 0;
    newNode->next = NULL;

    if (head == NULL){
        return newNode;
    }

    node* ptr = head;
    node* prev = NULL;
    while (ptr != NULL){
        // compare alphabetically
        int cmp = strcmp(ptr->word, newNode->word);
        // if the words are the same, free the newNode and increment counter
        if (cmp == 0){
            ptr->count++;
            free(newNode->word);
            free(newNode);
            pthread_mutex_unlock(&node_lock);
            return head;
        }
        if (cmp > 0) break;
        
        prev = ptr;
        ptr = ptr->next;
    }
    //if newNode is the new head
    if (prev == NULL) {
        newNode->next = ptr;
        head = newNode;
    }
    else {
        prev->next = newNode;
        newNode->next = ptr;
    }

    return head;
}

void freeList(node* head){
   node* ptr = head;

   while (head != NULL){
        ptr = head;
        head = head->next;
        free(ptr->word);
        free(ptr);
    }

}

void printList(node* head){

    node* ptr = head;

    while (ptr != NULL){
        printf("[%s] ", ptr->word);
        ptr = ptr->next;
    }
}
