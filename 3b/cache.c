#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "func.h"

typedef struct key_space {
	int offset;
	int len;
	int info;
	int release;
	char *key;
} key_space;

typedef struct table {
    char *filename;
    FILE *file;
    int offset;
} table;

typedef char bool;

typedef struct cache_item {
        char *key;
        int info;
        int release;

        int age_bits;
        bool recorded;
        bool deleted;
} cache_item;

typedef struct cache {
        cache_item *arr;
        int size;
        int top;
} cache;

cache *create_cache(int size)
{
        cache *tab_cache = (cache*)malloc(sizeof(cache));
        tab_cache->size = size;
        tab_cache->top = 0;
        tab_cache->arr = (cache_item*)malloc(sizeof(cache_item) * size);
        for (int i = 0; i < size; ++i) {
                tab_cache->arr[i].key = NULL;
        }
        return tab_cache;
}

int cache_is_empty(cache *tab_cache)
{
        return tab_cache->top == 0;
}

int cache_is_full(cache *tab_cache)
{
        return tab_cache->top == tab_cache->size;
}

void destroy_cache(cache *tab_cache)
{
        for (int i = 0; i < tab_cache->size; i++) {
                if (tab_cache->arr[i].key != NULL) {
                        free(tab_cache->arr[i].key);
                }
        }
        free(tab_cache->arr);
        free(tab_cache);
}

void print_cache(cache *tab_cache)
{
        for (int i = 0; i < tab_cache->top; ++i) {
                printf("%s. %d -- %d ||| age : %d, record : %d, deleted : %d\n", tab_cache->arr[i].key,
                        tab_cache->arr[i].info, tab_cache->arr[i].release, tab_cache->arr[i].age_bits,
                        tab_cache->arr[i].recorded, tab_cache->arr[i].deleted);
        }
}

int equals_cache(cache_item element, char *key, int info, int release)
{
        if (strcmp(element.key, key) == 0 && element.info == info && element.release == release)
                return 1;

        return 0;
}

int cache_delete_contains(cache *tab_cache, key_space *element)
{
        for (int i = 0; i < tab_cache->top; ++i) {
                if (equals_cache(tab_cache->arr[i], element->key, element->info, element->release) &&
                        tab_cache->arr[i].deleted == 1) {
                        tab_cache->arr[i].age_bits = tab_cache->arr[i].age_bits + 1;
                        return 1;
                }
        }
        return 0;
}

/*cache_item init_item(char *key, int info, int release, char flag)
{
        cache_item element;
        element.key = strdup(key);
        element.info = info;
        element.release = release;
        if (flag) {
                element.recorded = 1;
                element.deleted = 0;
        } else {
                element.deleted = 1;
                element.recorded = 0;
        }
        element.age_bits = 0;
        return element;
}*/

cache_item init_item(char *key, int info, int release, char flag, char search)
{
        cache_item element;
        element.key = strdup(key);
        element.info = info;
        element.release = release;
        if (flag) {
                element.recorded = 1;
                element.deleted = 0;
        } else {
                element.deleted = 1;
                element.recorded = 0;
        }
        element.age_bits = 0;
        if (search) {
        	element.deleted = 0;
        	element.recorded = 0;
        }	
        return element;
}

int index_of_younger(cache *tab_cache)
{
        int age = tab_cache->arr[0].age_bits, index = 0;
        for (int i = 0; i < tab_cache->top; ++i) {
                if (tab_cache->arr[i].age_bits < age) {
                        index = i;
                        age = tab_cache->arr[i].age_bits;
                        tab_cache->arr[i].age_bits = tab_cache->arr[i].age_bits + 1;
                }
        }
        return index;
}

int put_in_cache(table *tab, cache *tab_cache, char *key, int info, int cnt, char flag, char search)
{
        if (!cache_is_full(tab_cache)) {
                if (tab_cache->arr[tab_cache->top].key != NULL)
                        free(tab_cache->arr[tab_cache->top].key);
                tab_cache->arr[tab_cache->top] = init_item(key, info, cnt, flag, search);
                tab_cache->top = tab_cache->top + 1;
                return -1;
        }
        int index = index_of_younger(tab_cache);
        if (tab_cache->arr[index].recorded == 1) {
                add_element_file(tab, tab_cache->arr[index].key, tab_cache->arr[index].info,
                        tab_cache->arr[index].release);
        } else {
        		if (tab_cache->arr[index].deleted == 1) {
	                key_space *element = delete_one_file(tab, tab_cache->arr[index].key,
	                        tab_cache->arr[index].release);
	                if (element != NULL)
	                        destroy_element(element);
	            } else {
	            	//остановился здесь
	            	goto clear;
	            }
        }
        clear : free(tab_cache->arr[index].key);
        return index;
}

void print_cache_item(cache_item element)
{
        printf("%s. %d -- %d, %d | %d | %d\n", element.key, element.info, element.release,
                element.age_bits, element.recorded, element.deleted);
}

