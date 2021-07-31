
/*
strbuf library from assignment #2
Used to help tokenize each file
*/
typedef struct {
    size_t length;
    size_t used;
    char *data;
} strbuf_t;


//Initalizes strbuf, used starts at one because NULL-Terminator is added
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

int sb_append(strbuf_t *L, char item)
{
    if (L->used == L->length) {
	size_t size = L->length * 2;
	char *p = realloc(L->data, sizeof(char) * size);
    if (!p) return 1;

	L->data = p;
	L->length = size;
    }

    //strbuf is guarenteed to end with NULL-terminator
    //get replace NULL-terminator with item, and add null terminator at the end
    L->data[L->used-1] = item;
    L->data[L->used] = '\0';
    ++L->used;

    return 0;
}

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

void sb_destroy(strbuf_t *L)
{
    free(L->data);
}
