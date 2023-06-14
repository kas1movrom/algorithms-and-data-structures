#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#include "iterator.h"
#include "table.h"
#include "func.h"

#define eof -1
#define bytes_size 4
#define HASH_NUMBER 37
#define BYTE_SIZE 256

typedef struct item {
        unsigned int release;
        unsigned int key;
        char *info;
        struct item *next;
} item;

typedef struct key_space {
        item *head;
} key_space;

typedef struct table {
        key_space **arr;
        int size;
        int cnt;
} table;

int get_correct_info(char **info)
{
        if (*info == NULL)
                 return eof;

        if (strlen(*info) == 0)
    {
                while (strlen(*info) == 0)
                {
                        printf("Info cannot be empty!\n");
                        free(*info);
                        *info = readline(">");
        }
    }
    return 1;
}

int get_number_releases(int *release, int cnt)
{
        int n;
    while ((n = scanf("%d", release)) != 1 || *release < 0 || *release > cnt)
    {
        if (n == EOF)
        {
                        return eof;
        }
        printf("Enter the correct number of release!\n");
        scanf("%*c");
    }
    return 1;
}

int get_positive_int(int *key)
{
    int n;
    while ((n = scanf("%d", key)) != 1 || *key <= 0)
    {
                if (n == EOF)
                {
                        return eof;
                }
        printf("Enter positive int value!\n");
        scanf("%*c");
    }
    return 1;
}

int element_uncorrect(table *tab, unsigned int key, char *info)
{
        for (item *ptr = item_first(tab->arr[hash_function(key, tab)]);
                !item_done(ptr); ptr = item_next(ptr)) {
                if (ptr->key == key && strcmp(ptr->info, info) == 0)
                        return 1;
        }
        return 0;
}

void push_front_existing(key_space *list, char *info, int release, int key)
{
        item *ptr = list->head;
        item *element = create_item();
        init_item(release, info, key, ptr, element);
        list->head = element;
}

int destroy_arr(item **arr, int *size)
{
        for (int i = 0; i < *size; ++i)
        {
                free(arr[i]->info);
                free(arr[i]);
        }
        free(arr);
        return 1;
}

void remove_next(item *ptr)
{
        item *point = ptr->next;
        ptr->next = point->next;
        free(point->info);
        free(point);
}

int destroy_list(key_space *list)
{
        item *ptr = list->head;
        while (ptr != NULL)
        {
                free(ptr->info);
                item *next = ptr->next;
                free(ptr);
                ptr = next;
        }
        list->head = NULL;
        return 1;
}

void print_list(key_space *list)
{
        for (item *ptr = item_first(list); !item_done(ptr); ptr = item_next(ptr)) {
                printf("\t%d. %s -- %d\n", ptr->key, ptr->info, ptr->release);
        }
}

int list_is_empty(key_space *list)
{
        return item_done(list->head);
}

void push_front(key_space *list, char *info, int key)
{
        item *ptr = list->head;
        item *element = create_item();
        init_item(get_release(list, key), info, key, ptr, element);
        list->head = element;
}

int get_release(key_space *list, unsigned int key)
{
        int cnt = 0;
        for (item *ptr = item_first(list); !item_done(ptr);     ptr = item_next(ptr)) {
                if (ptr->key == key)
                {
                        cnt = ptr->release + 1;
                        break;
                }
        }
        return cnt;
}

void init_item(int release, char *info, int key, item *next, item *element)
{
        element->release = release;
        element->key = key;
        element->next = next;
        element->info = strdup(info);
}

item *create_item()
{
        item *element = (item*)malloc(sizeof(item));
        element->release = 0;
        element->info = NULL;
        element->key = 0;
        element->next = NULL;
        return element;
}

unsigned int hash_function(int key, table *tab) {
        int *bytes = (int*)calloc(sizeof(int), sizeof(int));
        int j = 0;
        while (key)
        {
                int ost = key % BYTE_SIZE;
                key /= BYTE_SIZE;
                bytes[j] = ost;
                j++;
        }
        unsigned int hash = 0;
        for (int i = 0; i < bytes_size; ++i) {
                hash = HASH_NUMBER * hash + bytes[i];
        }
        free(bytes);
        return hash % tab->size;
}

int table_is_empty(table *tab)
{
        int cnt = 0;
        for (int i = 0; i < tab->size; ++i) {
                if (list_is_empty(tab->arr[i]))
                        cnt++;
        }
        return cnt == tab->size;
}

int table_is_full(table *tab)
{
        if (tab->cnt == tab->size)
                return 1;

        return 0;
}
