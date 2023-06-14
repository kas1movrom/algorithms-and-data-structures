#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"
#include "func.h"
#include "iterator.h"

#define sep "|"

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

table *create_table(int size)
{
        table *tab = (table*)malloc(sizeof(table));
        tab->size = size;
        tab->cnt = 0;
        tab->arr = (key_space**)malloc(sizeof(key_space*) * tab->size);
        for (int i = 0; i < tab->size; ++i) {
                tab->arr[i] = (key_space*)malloc(sizeof(key_space));
                tab->arr[i]->head = NULL;
        }
        return tab;
}

int add_item(table *tab, int key, char *info)
{
        unsigned int hash = hash_function(key, tab);
        if (list_is_empty(tab->arr[hash]))
                tab->cnt++;

        push_front(tab->arr[hash], info, key);
        return hash;
}

int print_table(table *tab)
{
        int cnt = 0;
        for (int i = 0; i < tab->size; ++i) {
                printf("hash #%d:\n", i);
                if (list_is_empty(tab->arr[i])) {
                        printf("\the is empty!\n");
                        cnt++;
                } else {
                        print_list(tab->arr[i]);
                }
        }
        return cnt;
}

int destroy_table(table *tab)
{
        for (int i = 0; i < tab->size; ++i) {
                destroy_list(tab->arr[i]);
        }
        for (int i = 0; i < tab->size; ++i) {
                free(tab->arr[i]);
        }
        free(tab->arr);
        free(tab);
}

item **delete_all(table *tab, unsigned int key, int *size)
{
        unsigned int hash = hash_function(key, tab);
        int cnt = get_release(tab->arr[hash], key);
        *size = cnt;
        item **arr = (item**)malloc(sizeof(item*) * cnt);
        int i = 0;

        item *ptr = tab->arr[hash]->head;

        while (ptr->next != NULL)
        {
                if (ptr->next->key == key) {
                        arr[i] = create_item();
                        init_item(ptr->next->release, ptr->next->info, key, NULL, arr[i]);
                        ++i;
                        remove_next(ptr);
                } else {
                        ptr = ptr->next;
                }
        }

        ptr = tab->arr[hash]->head;
        if (ptr->key == key)
        {
                arr[i] = create_item();
                init_item(ptr->release, ptr->info, key, NULL, arr[i]);
                tab->arr[hash]->head = ptr->next;
                free(ptr->info);
                free(ptr);
        }

        if (list_is_empty(tab->arr[hash]))
                tab->cnt--;

        return arr;
}

key_space *search_all(table *tab, unsigned int key)
{
        unsigned int hash = hash_function(key, tab);
        key_space *list = (key_space*)malloc(sizeof(key_space));
        list->head = NULL;

        for (item *ptr = item_first(tab->arr[hash]); !item_done(ptr); ptr = item_next(ptr)) {
                if (ptr->key == key)
                {
                        push_front_existing(list, ptr->info, ptr->release, key);
                }
        }

        return list;
}

key_space *search_one(table *tab, unsigned int key, unsigned int release)
{
        unsigned int hash = hash_function(key, tab);
        key_space *list = (key_space*)malloc(sizeof(key_space));
        list->head = NULL;

        for (item *ptr = item_first(tab->arr[hash]); !item_done(ptr); ptr = item_next(ptr)) {
                if (ptr->key == key && ptr->release == release)
                {
                        push_front_existing(list, ptr->info, ptr->release, key);
                }
        }

        return list;
}

int table_cleaning(table *tab)
{
	int cnt = 0;
	for (int i = 0; i < tab->size; ++i) {
		if (!list_is_empty(tab->arr[i]))
		{
			item *ptr = tab->arr[i]->head;
			while (ptr != NULL)
			{
				item *element = ptr;
				while (element->next != NULL)
				{
					if (element->next->release < ptr->release && element->next->key == ptr->key) {
						++cnt;
						remove_next(element);
					} else {
						element = element->next;
					}
				}
				ptr = ptr->next;
			}
	        for (item *ptr = item_first(tab->arr[i]); !item_done(ptr); ptr = item_next(ptr)) {
	            if (ptr->release > 0)
	            {
	                ptr->release = 0;
	            }
	        }			
		}
	}
	return cnt;
}

char *my_fscanf(FILE *file)
{
        char *ptr = (char*)malloc(sizeof(char));
        char buf[81];
        int n, len = 0;
        *ptr = '\0';
        do
        {
                n = fscanf(file, "%80[^\n]", buf);
                if (n < 0)
                {
                        free(ptr);
                        ptr = NULL;
                        continue;
                }
                if (n == 0)
                {
                        fscanf(file, "%*c");
                }
                if (n > 0)
                {
                        len += (int)strlen(buf);
                        ptr = (char*)realloc(ptr, len + 1);
                        strcat(ptr, buf);
                }
        } while (n > 0);
        return ptr;
}

char* make_item(char *str, unsigned int *key)
{
        int cnt = 0;
        char *portion = strtok(str, sep);
        char *info;

        while (portion != NULL)
        {
                if (cnt == 0)
                {
                        *key = atoi(portion);
                        if (*key <= 0)
                        {
                                *key = -1;
                        }
                }
                if (cnt == 1)
                {
                        info = strdup(portion);
                }
                cnt++;
                portion = strtok(NULL, sep);
        }
        return info;
}

