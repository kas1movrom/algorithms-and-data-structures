#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"
#include "func.h"

typedef struct key_space {
    int offset;
    int len;
    int key;
    int release;
    char *info;
} key_space;

typedef struct table {
    char *filename;
    FILE *file;
    int offset;
    int size;
} table;


table *create_table()
{
    table *tab = (table*)malloc(sizeof(table));
    tab->filename = NULL;
    tab->file = NULL;
    tab->size = 0;
    tab->offset = (int)sizeof(int);
    return tab;
}


int destroy_table(table *tab)
{
    free(tab->filename);
    free(tab);
    return 1;
}


key_space **delete_all(table *tab, int key, int *size)
{
    if (table_is_empty(tab))
        return NULL;

    *size = find_cnt_release(tab, key);
    const int cnt = *size;
    key_space **arr = (key_space**)malloc(sizeof(key_space*) * cnt);
    for (int i = 0; i < cnt; i++) {
        arr[i] = delete_one(tab, key, i);
    }
    return arr;
}

key_space *delete_one(table *tab, int key, int release)
{
    if (table_is_empty(tab))
        return NULL;

    key_space *element = create_element();
    load_element(tab, element);
    if (element->key == key && release == element->release)
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
        if (key == item->key && item->release == release)
        {
            int two_off = item->offset;
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

int add_element(table *tab, int key, char *info)
{
    key_space *element = create_element();
    if (table_is_empty(tab)) {
        init_element(element, 0, (int)strlen(info), strdup(info), 0, key);
        print_element_in_file(tab, element);
        int tmp = ftell(tab->file);
        fseek(tab->file, tab->offset, SEEK_SET);
        element->offset = tmp;
        fwrite(&(element->offset), sizeof(int), 1, tab->file);
        fseek(tab->file, element->offset, SEEK_SET);
        destroy_element(element);
    } else {
        init_element(element, 0, (int)strlen(info), strdup(info), find_cnt_release(tab, key), key);
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

key_space *search_one(table *tab, int key, int release)
{
    int size = 0;
    key_space **arr = search_all(tab, key, &size);
    if (arr == NULL)
        return NULL;

    for (int i = 0; i < size; ++i) {
        if (arr[i]->release == release) {
            key_space *element = create_element();
            init_element(element, arr[i]->offset, arr[i]->len, strdup(arr[i]->info), arr[i]->release, arr[i]->key);
            destroy_arr(arr, size);
            return element;
        }
    }
    return NULL;
}

key_space **search_all(table *tab, int key, int *size)
{
    int count = find_cnt_release(tab, key);

    *size = count;
    if (count == 0)
        return NULL;

    fseek(tab->file, tab->offset, SEEK_SET);
    key_space **arr = (key_space**)malloc(sizeof(key_space*) * count);
    int target = tab->offset, cnt = 0;
    char ch;
    while (!feof(tab->file))
    {
        fseek(tab->file, target, SEEK_SET);
        key_space *element = create_element();
        load_element(tab, element);
        if (element->key == key)
        {
            arr[cnt] = create_element();
            init_element(arr[cnt], 0, 0, strdup(element->info), element->release, element->key);
            ++cnt;
        }
        fseek(tab->file, element->offset, SEEK_SET);
        target = element->offset;
        fread(&ch, sizeof(char), 1, tab->file);
        destroy_element(element);
    }
    return arr;
}

/*int table_clean(table *tab)
{
    if (table_is_empty(tab))
        return 0;

    int cnt = 0;
    fseek(tab->file, tab->offset, SEEK_SET);
    int target = tab->offset;
    char ch;
    while (!feof(tab->file))
    {
        fseek(tab->file, target, SEEK_SET);
        key_space *element = create_element();
        load_element(tab, element);
        if (element->release > 0)
        {
            key_space *item = delete_one(tab, element->key, element->release);
            destroy_element(item);
            ++cnt;
        }
        fseek(tab->file, element->offset, SEEK_SET);
        target = element->offset;
        fread(&ch, sizeof(char), 1, tab->file);
        destroy_element(element);
    }
    return cnt;
}*/

int table_clean(table *tab)
{
    if (table_is_empty(tab))
        return 0;

    int cnt = 0;
    fseek(tab->file, tab->offset, SEEK_SET);
    int target = tab->offset;
    char ch;
    int pos;
    while (!feof(tab->file))
    {
        fseek(tab->file, target, SEEK_SET);
        key_space *element = create_element();
        load_element(tab, element);
        if (element->release != -1)
        {
	        pos = ftell(tab->file);
	        int count = find_cnt_release(tab, element->key);
	        fseek(tab->file, pos, SEEK_SET);
	        if (count > 1)
	        {
	        	fseek(tab->file, tab->offset, SEEK_SET);
	        	int target1 = tab->offset;
	        	char ch;
	        	while (!feof(tab->file))
	        	{
	        		fseek(tab->file, target1, SEEK_SET);
	        		key_space *element1 = create_element();
	        		int pos2 = load_element(tab, element1);
	        		if (element1->key == element->key && element1->release < count - 1 && element1->release != -1)
	        		{
	        			element1->release = -1;
	        			fseek(tab->file, pos2, SEEK_SET);
	        			fwrite(&(element1->release), sizeof(int), 1, tab->file);	        			
	        		}
	        		fseek(tab->file, element1->offset, SEEK_SET);
	        		target1 = element1->offset;
	        		fread(&ch, sizeof(char), 1, tab->file);
	        		destroy_element(element1);
	        	}
	        }
        }
        fseek(tab->file, element->offset, SEEK_SET);
        target = element->offset;
        fread(&ch, sizeof(char), 1, tab->file);
        destroy_element(element);   	
    }

    fseek(tab->file, tab->offset, SEEK_SET);
    target = tab->offset;
    while (!feof(tab->file))
    {
        fseek(tab->file, target, SEEK_SET);
        key_space *element = create_element();
        load_element(tab, element);
        if (element->release == -1)
        {
            key_space *item = delete_one(tab, element->key, -1);
            destroy_element(item);
            ++cnt;        	
        }  
        fseek(tab->file, element->offset, SEEK_SET);
        target = element->offset;
        fread(&ch, sizeof(char), 1, tab->file);
        destroy_element(element);          	
    }

    fseek(tab->file, tab->offset, SEEK_SET);
    target = tab->offset;
    while (!feof(tab->file))
    {
        fseek(tab->file, target, SEEK_SET);
        key_space *element = create_element();
        int pos4 = load_element(tab, element);
        if (element->release != 0)
        {
            element->release = 0;
            fseek(tab->file, pos4, SEEK_SET);
            fwrite(&(element->release), sizeof(int), 1, tab->file);      	
        }  
        fseek(tab->file, element->offset, SEEK_SET);
        target = element->offset;
        fread(&ch, sizeof(char), 1, tab->file);
        destroy_element(element);          	
    }
    return cnt;
}

int show_table(table *tab, int hash)
{
    if (table_is_empty(tab))
        return 0;

    int target = tab->offset;
    char ch;
    fseek(tab->file, tab->offset, SEEK_SET);
    while (!feof(tab->file))
    {
        fseek(tab->file, target, SEEK_SET);
        key_space *element = create_element();
        load_element(tab, element);
        if (hash_function(element->key, tab->size) == hash)
            print_element(element);

        fseek(tab->file, element->offset, SEEK_SET);
        target = element->offset;
        fread(&ch, sizeof(char), 1, tab->file);
        destroy_element(element);
    }
    return 1;
}

