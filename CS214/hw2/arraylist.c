#include <stdlib.h>
#include <stdio.h>
#include "arraylist.h"

#ifndef DEBUG
#define DEBUG 0
#endif
//TODO CHECK COMPARISON AT LINE 64
int al_init(arraylist_t *L, size_t length)
{
    L->data = malloc(sizeof(int) * length);
    if (!L->data) return 1;

    L->length = length;
    L->used   = 0;

    return 0;
}

void al_destroy(arraylist_t *L)
{
    free(L->data);
}


int al_append(arraylist_t *L, int item)
{
    if (L->used == L->length) {
	size_t size = L->length * 2;
	int *p = realloc(L->data, sizeof(int) * size);
    if (!p) return 1;

	L->data = p;
	L->length = size;

	if (DEBUG) printf("Increased size to %lu\n", size);
    }

    L->data[L->used] = item;
    ++L->used;

    return 0;
}


int al_remove(arraylist_t *L, int *item)
{
    if (L->used == 0) return 1;

    --L->used;

    if (item) *item = L->data[L->used];
    
    return 0;
}

int al_insert(arraylist_t *list, int index, int item)
{

    if (index >= list->length || list->used == list->length){
        //realloc if index is outside of length or shifting doesn't fit
        size_t size = list->length * 2;
        //find out if length*2 > index + 1, use bigger, as length to reallocate
        if (index + 1 > size){
            size = index + 1;
        }
        int *p = realloc(list->data, sizeof(int) * size);
        //return 1 if cant allocate
        if (!p) return 1;
        list->data = p;
        list->length = size;
    }

        if (index >= list->used){
            //insert if theres no need for shifting
            list->data[index] = item;
            list->used = index + 1;
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