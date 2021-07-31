#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h> 
#include <sys/wait.h>
#include <stdbool.h>

#ifndef WWPATH
#define WWPATH "/ilab/users/gnp32/cs214/hw3/ww"
#endif


int isDirectory(char *name){
    struct stat data;
    
    //check for errors
    int error = stat(name, &data);
    if (error){
        perror(name);
        return 0;
    }
    
    //check if argv is a directory
    if (S_ISDIR(data.st_mode)){
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[]){
//TODO: check isDir
    int possibleNewLine = false;
    int Error = 0;
    for (int i = 2; i < argc; i++) {
        if (isDirectory(argv[i])){
            Error = 1;
            //Enchacement 1
            fprintf(stderr, "error: Argument is Directory\n");
            continue;
        }
        int fd[2];
        pipe(fd);
        pid_t pid = fork();         //call fork for every file

        if (pid == -1) {            //check for fork failure
            perror("fork");
            exit(EXIT_FAILURE);
        }
        //0 means child process
        else if (pid == 0){
            close(fd[0]);
            dup2(fd[1], 1);
            //call ww here
            execl(WWPATH, WWPATH, argv[1], argv[i], NULL);
            perror("execl");
            exit(EXIT_FAILURE);
        }
        //Parent process
        else {
            close(fd[1]);
            char buf[100];
            //Enchancement 3
            //only add a new line if ive seen atleast 2 files that contain text
            int bytes_read = read(fd[0], buf, 100);
            if (bytes_read > 0){
                if (possibleNewLine == true){
                    write(1, "\n", 1);
                }
                possibleNewLine = true;
            }
            // Do-while here since I already called read above
            do {
                write(1, buf, bytes_read);
            } while ((bytes_read = read(fd[0], buf, 100)) > 0);

            close(fd[0]);

            int wstatus;
            wait(&wstatus);
            //Enchancement 2 - checking exit status of ww, if it isnt EXIT_SUCCESS return EXIT_FAILURE at the end
            if (WEXITSTATUS(wstatus) != 0){
                Error = 1;
            }
        }
    }
    //checking for Directory error, or ww errors
    if (Error == 1)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
} 