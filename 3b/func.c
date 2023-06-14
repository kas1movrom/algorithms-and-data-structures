#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cache.h"
#include "table.h"

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

int init_table(char *filename, table *tab)
{
    tab->filename = filename;
    tab->file = fopen(tab->filename, "r+b");
    if (tab->file == NULL)
    {
        tab->file = fopen(tab->filename, "w+b");
        return 0;
    }
    return 1;
}

int file_is_empty(table *tab)
{
        char cnt = 0, check;
        fseek(tab->file, tab->offset, SEEK_SET);
        while (!feof(tab->file))
        {
                fread(&check, sizeof(char), 1, tab->file);
                ++cnt;
                if (cnt > 1) {
                        fseek(tab->file, tab->offset, SEEK_SET);
                        return 0;
                }
        }
        fseek(tab->file, tab->offset, SEEK_SET);
        return 1;
}

key_space *create_element()
{
    key_space *element = (key_space*)malloc(sizeof(key_space));
    element->offset = 0;
    element->info = 0;
    element->len = 0;
    element->release = 0;
    element->key = NULL;
    return element;
}

int load_element(table *tab, key_space *element)
{
    fread(&(element->offset), sizeof(int), 1, tab->file);
    fread(&(element->len), sizeof(int), 1, tab->file);
    element->key = (char*)malloc(sizeof(char) * (element->len + 1));
    fread(&(element->info), sizeof(int), 1, tab->file);
    int pos = ftell(tab->file);
    fread(&(element->release), sizeof(int), 1, tab->file);
    fread(element->key, element->len, sizeof(char), tab->file);
    element->key[element->len] = '\0';
    return pos;
}

void destroy_element(key_space *element)
{
        if (element->key != NULL)
                free(element->key);

        free(element);
}

void print_element_in_file(table *tab, key_space *element)
{
    fwrite(&(element->offset), sizeof(int), 1, tab->file);
    fwrite(&(element->len), sizeof(int), 1, tab->file);
    fwrite(&(element->info), sizeof(int), 1, tab->file);
    fwrite(&(element->release), sizeof(int), 1, tab->file);
    fwrite(element->key, element->len, sizeof(char), tab->file);
}



void init_element(key_space *element, int offset, int len, int info, int release, char *key)
{
    element->offset = offset;
    element->len = len;
    element->info = info;
    element->release = release;
    element->key = key;
}

void print_element(key_space *element)
{
    printf("%s. %d -- %d\n", element->key, element->info, element->release);
}

int add_element_file(table *tab, char *key, int info, int release)
{
        key_space *element = create_element();
        init_element(element, 0, (int)strlen(key), info, release, strdup(key));
        if (file_is_empty(tab)) {

        print_element_in_file(tab, element);
        int tmp = ftell(tab->file);
        fseek(tab->file, tab->offset, SEEK_SET);
        element->offset = tmp;
        fwrite(&(element->offset), sizeof(int), 1, tab->file);
        fseek(tab->file, element->offset, SEEK_SET);
        destroy_element(element);

        } else {

                fseek(tab->file, 0, SEEK_END);
        int pos = ftell(tab->file);
        print_element_in_file(tab, element);
        int tmp = ftell(tab->file);
        element->offset = tmp;
        fseek(tab->file, pos, SEEK_SET);
        fwrite(&(element->offset), sizeof(int), 1, tab->file);
        fseek(tab->file, element->offset, SEEK_SET);
        destroy_element(element);

        }
        return 1;
}

int find_cnt_release(table *tab, char *key, cache *tab_cache)
{
        int cnt = 0;
        for (int i = 0; i < tab_cache->top; ++i) {
                if (strcmp(tab_cache->arr[i].key, key) == 0 && tab_cache->arr[i].recorded == 1) {
                        cnt++;
                        tab_cache->arr[i].age_bits = tab_cache->arr[i].age_bits + 1;
                }
        }

        if (!file_is_empty(tab))
        {
                int target = tab->offset;
                char ch;
                fseek(tab->file, tab->offset, SEEK_SET);
                while (!feof(tab->file))
                {
                        fseek(tab->file, target, SEEK_SET);
                        key_space *element = create_element();
                        load_element(tab, element);
                        if (strcmp(element->key, key) == 0 && cache_delete_contains(tab_cache, element) == 0)
                                ++cnt;

                        fseek(tab->file, element->offset, SEEK_SET);
                        target = element->offset;
                        fread(&ch , sizeof(char), 1, tab->file);
                        destroy_element(element);
                }
        }
        return cnt;
}

key_space *delete_one_file(table *tab, char *key, int release)
{
        if (file_is_empty(tab))
                return NULL;

        fseek(tab->file, tab->offset, SEEK_SET);
        key_space *element = create_element();
        load_element(tab, element);
        if (strcmp(element->key, key) == 0 && release == element->release)
        {
                tab->offset = element->offset;
                return element;
        }

        destroy_element(element);
    int off = tab->offset;
    int pos = 0, pos1 = 0;
    fseek(tab->file, tab->offset, SEEK_SET);
    char flag = 1;
    while (flag)
    {
        pos = ftell(tab->file);
        fread(&off, sizeof(int), 1, tab->file);
        fseek(tab->file, off, SEEK_SET);
        pos1 = ftell(tab->file);
        key_space *item = create_element();
        load_element(tab, item);
        if (strcmp(key, item->key) == 0 && item->release == release)
        {
            int two_off = ftell(tab->file);
            fseek(tab->file, pos, SEEK_SET);
            fwrite(&two_off, sizeof(int), 1, tab->file);
            flag = 0;
            return item;
        }
        destroy_element(item);
        fseek(tab->file, pos1, SEEK_SET);
    }
    return NULL;
}

void destroy_array(key_space **arr, int size)
{
        for (int i = 0; i < size; ++i) {
                destroy_element(arr[i]);
        }
        free(arr);
}

void correct_cnt_release(table *tab, cache *tab_cache, char *key, int release)
{
        if (table_is_empty(tab, tab_cache) != 1 && find_cnt_release(tab, key, tab_cache) != 0)
        {
                for (int i = 0; i < tab_cache->top; ++i) {
                        if (strcmp(tab_cache->arr[i].key, key) == 0 && tab_cache->arr[i].recorded == 1 &&
                                tab_cache->arr[i].release > release) {
                                tab_cache->arr[i].age_bits = tab_cache->arr[i].age_bits + 1;
                                tab_cache->arr[i].release = tab_cache->arr[i].release - 1;
                        }
                }

                if (!file_is_empty(tab))
                {
                        fseek(tab->file, tab->offset, SEEK_SET);
                        int target = tab->offset;
                        char ch;
                        while (!feof(tab->file))
                        {
                                fseek(tab->file, target, SEEK_SET);
                                key_space *element = create_element();
                                int pos = load_element(tab, element);
                                if (strcmp(element->key, key) == 0 && element->release > release &&
                                        cache_delete_contains(tab_cache, element) == 0) {
                                        element->release = element->release - 1;
                                        fseek(tab->file, pos, SEEK_SET);
                                        fwrite(&(element->release), sizeof(int), 1, tab->file);
                                }
                                fseek(tab->file, element->offset, SEEK_SET);
                                target = element->offset;
                                fread(&ch, sizeof(char), 1, tab->file);
                                destroy_element(element);
                        }
                }
        }
}

key_space **load_in_file(table *tab, cache *tab_cache, int *size)
{
         if (table_is_empty(tab, tab_cache))
                return NULL;

         key_space **arr = (key_space**)malloc(sizeof(key_space*) * 100);
         int cnt = 0;
         for (int i = 0; i < tab_cache->top; ++i) {
                if (tab_cache->arr[i].recorded == 1) {
                        arr[cnt] = create_element();
                        init_element(arr[cnt], 0, 0, tab_cache->arr[i].info, tab_cache->arr[i].release,
                                strdup(tab_cache->arr[i].key));
                        ++cnt;
                }
         }

         if (file_is_empty(tab)) {
                *size = cnt;
                arr = (key_space**)realloc(arr, sizeof(key_space*) * (*size));
                return arr;
         }

         int target = tab->offset;
         char ch;
         fseek(tab->file, tab->offset, SEEK_SET);
         while (!feof(tab->file))
         {
                fseek(tab->file, target, SEEK_SET);
                key_space *element = create_element();
                load_element(tab, element);
                if (!cache_delete_contains(tab_cache, element)) {
                        arr[cnt] = create_element();
                        init_element(arr[cnt], 0, 0, element->info, element->release, strdup(element->key));
                        ++cnt;
                }
                fseek(tab->file, element->offset, SEEK_SET);
                target = element->offset;
                fread(&ch, sizeof(char), 1, tab->file);
                destroy_element(element);
         }
         *size = cnt;
         arr = (key_space**)realloc(arr, sizeof(key_space*) * (*size));
         return arr;
}

void create_final_file(table *tab, key_space **arr, int size)
{
        table *tab1 = create_table();
        tab1->filename = tab->filename;
        tab1->file = fopen(tab1->filename, "w+b");
        tab1->offset = 0;
        for (int i = 0; i < size; ++i) {
                add_element_file(tab1, arr[i]->key, arr[i]->info, arr[i]->release);
        }
        destroy_array(arr, size);
        fclose(tab1->file);
        destroy_table(tab);
        free(tab1);
}

