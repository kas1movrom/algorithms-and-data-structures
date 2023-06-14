#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"

typedef struct item {
    unsigned int info;
} item;

typedef struct key_space {
    char *key;
    unsigned int release;
    item *point;
    struct key_space *next;
} key_space;

typedef struct table{
    key_space *elements;
} table;

typedef int (*t_func) (table *);


int table_is_empty(table *tab)
{
    if (tab->elements == NULL) return 1;
    return 0;
}

table *create_table()
{
    table *tab = (table*)malloc(sizeof(table));
    tab->elements = NULL;
    return tab;
}

key_space *create_element(char *key, unsigned int info)
{
    key_space *element = (key_space*)malloc(sizeof(key_space));
    element->key = key;
    element->point = (item*)malloc(sizeof(item));
    element->point->info = info;
    return element;
}

key_space *create_copy(key_space *element)
{
        char *str = strdup(element->key);
    key_space *ptr = create_element(str, element->point->info);
    ptr->release = element->release;
    return ptr;
}

int equals(key_space *ptr1, key_space *ptr2)
{
    if (strcmp(ptr1->key, ptr2->key) == 0 && ptr1->release == ptr2->release)
        return 1;

    return 0;
}

void dias(table *tab, key_space *element)
{
    key_space *ptr = tab->elements;
    while (!equals(ptr, element))
    {
        if (strcmp(ptr->key, element->key) == 0)
        {
            (ptr->release)--;
        }
        ptr = ptr->next;
    }
}

int cnt_key(table *tab, char *key)
{
    if (table_is_empty(tab)) return 0;

    int cnt = 0;
    for (key_space *ptr = element_first(tab); !element_done(ptr);
    				ptr = element_next(ptr)) {
    	if (!strcmp(ptr->key, key))
    		cnt++;					
    }
    return cnt;
}


unsigned int add_element_exising(table *tab, key_space *element)
{
    if (table_is_empty(tab)) {
        tab->elements = element;
        element->next = NULL;
    } else {
        key_space *ptr = tab->elements;
        tab->elements = element;
        element->next = ptr;
    }
    return element->point->info;
}

int element_is_here(table *tab, char *key)
{
    for (key_space *ptr = element_first(tab); !element_done(ptr);
    				ptr = element_next(ptr)) {
    	if (!strcmp(ptr->key, key))
    		return 1;					
    }
    return 0;
}

key_space *element_first(table *tab)
{
	return tab->elements;
}

int element_done(key_space *element)
{
	return element == NULL;
}

key_space *element_next(key_space *element)
{
	return element->next;
}

int element_done_next(key_space *element)
{
	return element->next == NULL;
}
