#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cache.h"
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

table *create_table()
{
    table *tab = (table*)malloc(sizeof(table));
    tab->filename = NULL;
    tab->file = NULL;
    tab->offset = 0;
    return tab;
}

int destroy_table(table *tab)
{
    free(tab->filename);
    free(tab);
    return 1;
}

int table_is_empty(table *tab, cache *tab_cache)
{
        for (int i = 0; i < tab_cache->top; ++i) {
                //if (tab_cache->arr[i].recorded == 1) {
                if (tab_cache->arr[i].recorded || (tab_cache->arr[i].recorded == 0 && tab_cache->arr[i].deleted == 0)) {
                        tab_cache->arr[i].age_bits = tab_cache->arr[i].age_bits + 1;
                        return 0;
                }
        }

        if (file_is_empty(tab))
                return 1;

        int target = tab->offset;
        char ch, flag = 1;
        fseek(tab->file, tab->offset, SEEK_SET);
        while (!feof(tab->file))
        {
                fseek(tab->file, target, SEEK_SET);
                key_space *element = create_element();
                load_element(tab, element);
                if (!cache_delete_contains(tab_cache, element))
                        flag = 0;

                fseek(tab->file, element->offset, SEEK_SET);
                target = element->offset;
                fread(&ch , sizeof(char), 1, tab->file);
                destroy_element(element);
        }
        return flag;
}

int add_element(table *tab, cache *tab_cache, char *key, int info)
{
        int cnt = find_cnt_release(tab, key, tab_cache);
        int res = put_in_cache(tab, tab_cache, key, info, cnt, 1, 0); // остановился на этом
        if (res != -1)
                tab_cache->arr[res] = init_item(key, info, cnt, 1, 0);
        return 1;
}

key_space **search_all(table *tab, cache *tab_cache, char *key, int size)
{
        int cnt = 0;
        key_space **arr = (key_space**)malloc(sizeof(key_space*) * (size));
        for (int i = 0; i < tab_cache->top; ++i) {
                if (strcmp(tab_cache->arr[i].key, key) == 0 && tab_cache->arr[i].recorded == 1) {
                        tab_cache->arr[i].age_bits = tab_cache->arr[i].age_bits + 1;
                        arr[cnt] = create_element();
                        init_element(arr[cnt], 0, 0, tab_cache->arr[i].info, tab_cache->arr[i].release,
                                strdup(tab_cache->arr[i].key));
                        ++cnt;
                }
        }
        if (file_is_empty(tab))
                return arr;

        fseek(tab->file, tab->offset, SEEK_SET);
    int target = tab->offset;
    char ch;
    while (!feof(tab->file))
    {
        fseek(tab->file, target, SEEK_SET);
        key_space *element = create_element();
        load_element(tab, element);
        if (strcmp(element->key, key) == 0 && cache_delete_contains(tab_cache, element) == 0)
        {
            arr[cnt] = create_element();
            init_element(arr[cnt], 0, 0, element->info, element->release, strdup(element->key));
            cnt++;
        }
        fseek(tab->file, element->offset, SEEK_SET);
        target = element->offset;
        fread(&ch, sizeof(char), 1, tab->file);
        destroy_element(element);
    }
    return arr;
}

key_space *search_one(table *tab, char *key, int release, cache *tab_cache)
{
        int size = find_cnt_release(tab, key, tab_cache);
        key_space **arr =  search_all(tab, tab_cache, key, size);
        for (int i = 0; i < size; ++i) {
                if (arr[i]->release == release) {
                        key_space *element = create_element();
                        init_element(element, arr[i]->offset, arr[i]->len, arr[i]->info, arr[i]->release,
                                strdup(arr[i]->key));
                        destroy_array(arr, size);
                        return element;
                }
        }
        return NULL;
}

key_space *delete_one(table *tab, cache *tab_cache, char *key, int release)
{
        for (int i = 0; i < tab_cache->top; ++i) {
                if (strcmp(tab_cache->arr[i].key, key) == 0 && tab_cache->arr[i].release == release &&
                        tab_cache->arr[i].recorded == 1) {
                        key_space *element =  create_element();
                        init_element(element, 0, 0, tab_cache->arr[i].info, tab_cache->arr[i].release,
                                strdup(key));
                        for (int j = i; j < tab_cache->top - 1; j++) {
                                free(tab_cache->arr[j].key);
                                tab_cache->arr[j] = init_item(strdup(tab_cache->arr[j + 1].key),
                                        tab_cache->arr[j + 1].info, tab_cache->arr[j + 1].release,
                                        tab_cache->arr[j + 1].recorded, 0);
                                tab_cache->arr[j].age_bits = tab_cache->arr[j + 1].age_bits + 1;
                        }
                        tab_cache->top = tab_cache->top - 1;
                        return element;
                }
        }

    fseek(tab->file, tab->offset, SEEK_SET);
    key_space *result;
    int target = tab->offset;
    char ch;
    while (!feof(tab->file))
    {
        fseek(tab->file, target, SEEK_SET);
        key_space *element = create_element();
        load_element(tab, element);
        if (strcmp(element->key, key) == 0 && element->release == release &&
                cache_delete_contains(tab_cache, element) == 0) {
                result = element;
                break;
        }
        fseek(tab->file, element->offset, SEEK_SET);
        target = element->offset;
        fread(&ch, sizeof(char), 1, tab->file);
        destroy_element(element);
    }

    int res = put_in_cache(tab, tab_cache, result->key, result->info, result->release, 0, 0);
    if (res != -1)
        tab_cache->arr[res] = init_item(result->key, result->info, result->release, 0, 0);

    return result;


}

key_space **delete_all(table *tab, cache *tab_cache, char *key, int size)
{
        key_space **arr = (key_space**)malloc(sizeof(key_space*) * size);
        for (int i = 0; i < size; ++i) {
                arr[i] = delete_one(tab, tab_cache, key, 0);
                correct_cnt_release(tab, tab_cache, key, 0);
        }
        return arr;
}

int show_table(table *tab, cache *tab_cache)
{
        for (int i = 0; i < tab_cache->top; ++i) {
                if (tab_cache->arr[i].recorded == 1) {
                        printf("%s. %d -- %d\n", tab_cache->arr[i].key, tab_cache->arr[i].info,
                                tab_cache->arr[i].release);
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
                        if (cache_delete_contains(tab_cache, element) == 0)
                                printf("%s. %d -- %d\n", element->key, element->info, element->release);

                        fseek(tab->file, element->offset, SEEK_SET);
                        target = element->offset;
                        fread(&ch , sizeof(char), 1, tab->file);
                        destroy_element(element);
                }
        }
        return 1;
}
