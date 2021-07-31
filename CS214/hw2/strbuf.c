#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "strbuf.h"

#ifndef DEBUG
#define DEBUG 0
#endif

//Initalizes strbuf, used starts at once because NULL-Terminator is added
//Length 0 is not valid
int sb_init(strbuf_t *L, size_t length)
{
    //Length 0 is not valid
    if (length == 0) return 1;

    L->data = malloc(sizeof(char) * length);
    if (!L->data) return 1;

    L->length = length;
    L->used   = 1;

    L->data[0] = '\0';
    return 0;
}

void sb_destroy(strbuf_t *L)
{
    free(L->data);
}


int sb_append(strbuf_t *L, char item)
{
    if (L->used == L->length) {
	size_t size = L->length * 2;
	char *p = realloc(L->data, sizeof(char) * size);
    if (!p) return 1;

	L->data = p;
	L->length = size;

	if (DEBUG) printf("Increased size to %lu\n", size);
    }

    //strbuf is guarenteed to end with NULL-terminator
    //get replace NULL-terminator with item, and add null terminator at the end
    L->data[L->used-1] = item;
    L->data[L->used] = '\0';
    ++L->used;

    return 0;
}


int sb_remove(strbuf_t *L, char *item)
{
    //cannot remove only the NULL-Terminator
    if (L->used == 1) return 1;
    
    --L->used;
    //remove NULL-terminator then replace last char w NULL-terminator
    L->data[L->used-1] = '\0';

    if (item) *item = L->data[L->used];
    //fixed typo
    return 0;
}

//insert doesn't not consider any existing NULL-terminators, 
//it only adds one if the given index is now the last item
int sb_insert(strbuf_t *list, int index, char item)
{

    if (index >= list->length || list->used == list->length){
        //realloc if index is outside of length or shifting doesn't fit
        size_t size = list->length * 2;
        //find out if length*2 > index + 2, use the greater one as length to reallocate
        //+2 accounts for NULL-Terminator
        if (index + 2 > size){
            size = index + 2;
        }
        char *p = realloc(list->data, sizeof(char) * size);
        //return 1 if cant allocate
        if (!p) return 1;
        list->data = p;
        list->length = size;
    }

        if (index >= list->used){
            //insert if theres no need for shifting
            list->data[index] = item;
            list->data[index+1] = '\0';
            //+2 accounts for NULL-Terminator at the end
            list->used = index + 2;
        }
        else {
            //shift then insert
            for (int i = list->used - 1; i >= index; i--){
                list->data[i + 1] = list->data[i];
            }
            list->data[index] = item;
            list->used++;
            
        }

    return 0;

}
//concat just uses append for the length of the string
//check the return value of append incase there is an error
int sb_concat(strbuf_t *sb, char *str)
{ 
    size_t str_length = strlen(str);
    for (int i = 0; i < str_length; i++){
        if (sb_append(sb, str[i]) == 1){
            return 1;
        }
    }
    return 0;
}