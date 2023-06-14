#include <stdio.h>
#include "iterator.h"

typedef struct item {
        unsigned int release;
        unsigned int key;
        char *info;
        struct item *next;
} item;

typedef struct key_space {
        item *head;
} key_space;

item *item_first(key_space *list)
{
        return list->head;
}

int item_done(item *element)
{
        return element == NULL;
}

item *item_next(item *element)
{
        return element->next;
}
