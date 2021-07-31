#include "compare.h"

//TODO: ALL error checks
/* check: 
        pthread create
        pthread join
        mutex init
        TODO: !! ALL PDF ERRORS STATED !!
            AKA IF LESS THAN 1 WORD IN FILE AND STUFF
*/
int main(int argc, char* argv[]){

    if (argc <= 1){
        fprintf(stderr, "Not enough Arguments\n");
        exit(EXIT_FAILURE);
    }
    
    
    fileSuffix = malloc(sizeof(char) * 5);
    mallocErr(fileSuffix);
    memcpy(fileSuffix, ".txt", 5);
    
    for (int i = 1; i < argc; i++){
        //if arguement starts with a - it is an optional arg
        if (argv[i][0] == '-'){
            if (argv[i][1] == 'd' || argv[i][1] == 'f' || argv[i][1] == 'a'){
                //if strlen of input is < 3 that means it is missing an arguement. Eg: [-d] is missing a number
                if (strlen(argv[i]) < 3){
                    fprintf(stderr, "Missing argument\n");
                    exit(EXIT_FAILURE);
                }
                //next character is the given integer and must be >= 1
                int temp = atoi(&argv[i][2]);
                if (temp < 1){
                    fprintf(stderr, "Invalid input\n");
                    exit(EXIT_FAILURE);
                }
                switch(argv[i][1]){
                    case 'd':
                        dir_threads = temp;
                        break;
                    case 'f':
                        file_threads = temp;
                        break;
                    case 'a':
                        anal_threads = temp;
                        break;
                }
            }
            //for -s (file name suffix arg)
            else if (argv[i][1] == 's'){
                //is the arguement is less than 2 characters, then the file name suffix is an empty string
                if (strlen(argv[i]) < 2){
                    fileSuffix = "";
                }
                else {
                    fileSuffix = realloc(fileSuffix, strlen(argv[i]) - 1);
                    memcpy(fileSuffix, &argv[i][2], strlen(argv[i]) - 1);
                }
            }
        }
    }
  
    pthread_t* dir_tids = malloc(sizeof(pthread_t) * dir_threads);
    mallocErr(dir_tids);
    dir_init(&D, dir_threads);
    
    pthread_t* file_tids = malloc(sizeof(pthread_t) * file_threads);
    mallocErr(file_tids);
    file_init(&F, file_threads);

    //Create file threads
    for (int i = 0; i < file_threads; i++){
        pthread_create(&file_tids[i], NULL, fileThread, NULL);
    }

    //Load arguements into its respective queue
    for (int i = 1; i < argc; i++){
        //filter out optional args
        if (argv[i][0] != '-'){
            if (isDirectory(argv[i]) && argv[i][0] != '.'){
                dir_enqueue(&D, argv[i], strlen(argv[i])+1);
            }
            else if (ignoreFileName(argv[i])){
                //quick file open check
                int err = open(argv[i], O_RDWR);
                if (err == -1){ perror(argv[i]); returnError = true; continue; }
                close(err);
                file_enqueue(&F, argv[i], strlen(argv[i])+1);
            }
        }
    }

    //Start directory threads
    for (int i = 0; i < dir_threads; i++){
        pthread_create(&dir_tids[i], NULL, directoryThread, NULL);
    }
    //Join directory threads
    for (int i = 0; i < dir_threads; i++){
        pthread_join(dir_tids[i], NULL);
    }
    //Join file threads
    for (int i = 0; i < file_threads; i++){
        pthread_join(file_tids[i], NULL);
    }
    if (num_of_files < 2){
        fprintf(stderr, "ERROR: Found less than 2 files, exiting\n");
        exit(EXIT_FAILURE);
    }
    comparisons = (num_of_files) * (num_of_files - 1) / 2;
    jsd_array = malloc(sizeof(jsd_struct) * comparisons);
    mallocErr(jsd_array);
    global_ptr1 = head;    
    global_ptr2 = head->next;    


    pthread_t* anal_tids = malloc(sizeof(pthread_t) * anal_threads);
    mallocErr(anal_tids);

    
    work_load = (comparisons + anal_threads - 1) / anal_threads;
    for (int i = 0; i < anal_threads; i++){
        int *arg = malloc(sizeof(*arg));
        mallocErr(arg);
        *arg = i * work_load;
        pthread_create(&anal_tids[i], NULL, analThread, arg);

    }
    
     for (int i = 0; i < anal_threads; i++){
        pthread_join(anal_tids[i], NULL);
    }

    //Sort Final JSD  array
    qsort(jsd_array, comparisons, sizeof(jsd_struct), compare);
    //output
    printfJSD(jsd_array, comparisons);
    //free everything!
    freeJSD_struct(jsd_array, comparisons);
    free(fileSuffix);
    free(dir_tids);
    free(file_tids);
    free(anal_tids);
    free(jsd_array);
    freewfd_repo(head);
    pthread_mutex_destroy(&tokenizer_lock);
    dir_destroy(&D);
    file_destroy(&F);

    if (returnError)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

void* directoryThread(void* arg){

    while (D.count != 0 || D.active_threads != 0){
        
        char* path = dir_dequeue(&D, &F);
        if (path == NULL){
            return NULL;
        }

        if (isDirectory(path)){
            DIR *dirp;
            struct dirent *de;

            if (!(dirp = opendir(path))){
                closedir(dirp);
                returnError = false;
                perror(path);
            }
            while ((de = readdir(dirp)) != NULL) {
                //if its a directory add it to directory Queue
                if (de->d_type == DT_DIR && de->d_name[0] != '.') {
                    char temp[1024];
                    snprintf(temp, sizeof(temp), "%s/%s", path, de->d_name);
                    dir_enqueue(&D, temp, strlen(temp) + 1);
                }
                else if (de->d_type != DT_DIR && ignoreFileName(de->d_name) && de->d_name[0] != '.') {
                    char temp[1024];
                    snprintf(temp, sizeof(temp), "%s/%s", path, de->d_name); 
                    int err = open(temp, O_RDWR); //quick open file err
                    if (err == -1){ perror(temp); returnError = true; continue;}
                    close(err);
                    file_enqueue(&F, temp, strlen(temp) + 1);
                }
            }
            closedir(dirp);
        }
        free(path);
    }
    return NULL;
}

void* fileThread(void* arg){

    lock(&file_lock);
    while (F.count != 0 || D.active_threads != 0){
        
        char* path = file_dequeue(&F);
        if (path == NULL){
            unlock(&file_lock);
            return NULL;
        }
        //tokenize also computes wfd
        node* word_list = tokenize(path);
        //Add to WFD Repo
        //if word list is empty
        if (word_list != NULL)
            head = wfd_repo_insert(head, path, strlen(path) + 1 , word_list->overall_word_count, word_list);
        else
            head = wfd_repo_insert(head, path, strlen(path) + 1 , 0, word_list);
        
        free(path);
    }
    unlock(&file_lock);
    return NULL;
}

void* analThread(void* index){

    lock(&anal_lock);
    int i = *((int *) index);

    wfd_repo* f1 = global_ptr1;    
    wfd_repo* f2 = global_ptr2;
    int finish = min(i + work_load, comparisons);


    while (f1 != NULL){
        while (f2 != NULL){
           // printf("i: %d\n", i);
            jsd_array[i].file1 = malloc(sizeof(char) * f1->filePath_length);
            mallocErr(jsd_array[i].file1);
            memcpy(jsd_array[i].file1, f1->filePath, f1->filePath_length);
          
            jsd_array[i].file2 = malloc(sizeof(char) * f2->filePath_length);
            mallocErr(jsd_array[i].file2);
            memcpy(jsd_array[i].file2, f2->filePath, f2->filePath_length);
            
            jsd_array[i].word_count = f1->total_word_count + f2->total_word_count;
            jsd_array[i].JSD = calculateJSD(f1->word_list, f2->word_list);
           
            f2 = f2->next;
            i++;
            global_ptr1 = f1;
            global_ptr2 = f2;
            if (i >= finish){
                free(index);
                unlock(&anal_lock);
                return NULL;
            }
        }
        f1 = f1->next;
        if (f1 != NULL){
            f2 = f1->next;
        }
    }

    free(index);
    unlock(&anal_lock);
    return NULL;
}



double calculateJSD(node* file1, node* file2){

 
    node* ptr1 = file1;
    node* ptr2 = file2;
    double jsd;
    double kld_file1 = 0;
    double kld_file2 = 0;

    while (ptr1 != NULL && ptr2 != NULL){
        int cmp = strcmp(ptr1->word, ptr2->word);
        //if both words are the same
        if (cmp == 0){
            double meanFreq = (ptr1->frequency + ptr2->frequency) * 0.5;
            kld_file1 += (ptr1->frequency) * log2(ptr1->frequency/meanFreq);
            kld_file2 += (ptr2->frequency) * log2(ptr2->frequency/meanFreq);
            ptr1 = ptr1->next;
            ptr2 = ptr2->next;
        }
        else if (cmp <  0){
            double meanFreq = (ptr1->frequency + 0) * 0.5;
            kld_file1 += (ptr1->frequency) * log2(ptr1->frequency/meanFreq);
           ptr1 = ptr1->next;
        }
        else {
            double meanFreq = (0 + ptr2->frequency) * 0.5;
            kld_file2 += (ptr2->frequency) * log2(ptr2->frequency/meanFreq);
            ptr2 = ptr2->next;
        }
    }
    //if ptr1 finishes before ptr2 then we finish ptr2
    if (ptr1 == NULL){
        while (ptr2 != NULL){
            double meanFreq = (0 + ptr2->frequency) * 0.5;
            kld_file2 += (ptr2->frequency) * log2(ptr2->frequency/meanFreq);
            ptr2 = ptr2->next;
        }

    }//vice versa
    else if (ptr2 == NULL){
        while (ptr1 != NULL){
            double meanFreq = (ptr1->frequency + 0) * 0.5;
            kld_file1 += (ptr1->frequency) * log2(ptr1->frequency/meanFreq);
            ptr1 = ptr1->next;
        }

    }

    jsd = sqrt((0.5 * kld_file1) + (0.5 * kld_file2));
    return jsd;
}


node* tokenize(char* file) {

    //this lock deals with locking word_list/node as well
    lock(&tokenizer_lock);
    char* fileName = file;
    int fd = open(fileName, O_RDONLY);
    char buf[BUFSIZE];
    strbuf_t token;
    sb_init(&token, 10);
    int bytes_read;
    int word_count = 0;
    node* word_list = NULL;

    while((bytes_read = read(fd, buf, BUFSIZE)) != 0) {

        for (int i = 0; i < bytes_read; i++) {
            //if char is an ' ignore it
            if (!isalpha(buf[i]) && !isdigit(buf[i]) && !isspace(buf[i]) && buf[i] != '-') continue;

            //if we hit a delim and we have characters in our sb then send it
            if (isspace(buf[i]) && token.used > 1){
                word_list = insert(word_list, token.data, token.used);
                sb_destroy(&token);
                sb_init(&token, 10);
                word_count++;
            }
            //adds a valid char to sb
            else if (!isspace(buf[i])){
                char c = tolower(buf[i]);
                sb_append(&token, c);
            }
        }
    }
    //sends the final sb incase we didn't end on a delim
    if (token.used > 1) {
        word_list = insert(word_list, token.data, token.used);
        word_count++;
    }
    sb_destroy(&token);
    //calculate the WFD for this file
    if (word_list != NULL)
        word_list->overall_word_count = word_count;
    word_list = calculateWFD(word_list, word_count);
    close(fd);
    unlock(&tokenizer_lock);
    return word_list;
}


//TODO: check if we should use isalpha() and isdigit()
int isSymbol(char c) {

    //'-' is considered a valid character
    switch(c) {
        case '.':
        case ',':
        case ';':
        case '!':
        case '?':
        case '@':
        case '#':
        case '$':
        case '%':
        case '^':
        case '&':        
        case '*':        
        case '(':        
        case ')':        
        case '_':        
        case '+':        
        case '=':        
        case '[':        
        case ']':        
        case '{':        
        case '}':        
        case '/':        
        case '"':        
        case '\\':
        case '~':
        case '`':
        case '\'':
            return true;
    }
    return false;
}
//Calculates WFD for each word in a list
node* calculateWFD(node* word_list, int word_count){

    node* ptr = word_list;

    while (ptr != NULL){
        ptr->frequency = (double)(ptr->count) / word_count;
        //FREQUENCY must be between [0,1]
        assert(ptr->frequency >= 0 && ptr->frequency <= 1);
        ptr = ptr->next;
    }
    return word_list;
}

//checks if we ignore filename
int ignoreFileName(char* file){

    if (strlen(fileSuffix) == 0) return true;
    size_t length = strlen(file);
    size_t suffix_length = strlen(fileSuffix);

    return suffix_length <= length && !strcmp(file + length - suffix_length, fileSuffix);
}

int isDirectory(char* dirName){
    struct stat data;

    stat(dirName, &data);
   
    //check if its is a directory
    if (S_ISDIR(data.st_mode)){
        return true;
    }
    return false;
}



//compare function for qSort
int compare(const void *a, const void *b) {
    const jsd_struct *pA = (jsd_struct*)a;
    const jsd_struct *pB = (jsd_struct*)b;

    return (pB->word_count - pA->word_count);

}