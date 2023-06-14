#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#include "func.h"
#include "table.h"

#define BYTES_SIZE 4
#define ONE_BIT 256
#define HASH_NUMBER 37
#define eof -1

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

key_space **load_in_file(table *tab, int *size)
{
    if (table_is_empty(tab))
        return NULL;

    int target = tab->offset;
    key_space **arr = (key_space**)malloc(sizeof(key_space*) * 100);
    int cnt = 0;
    char ch;
    fseek(tab->file, tab->offset, SEEK_SET);
    while (!feof(tab->file))
    {
        fseek(tab->file, target, SEEK_SET);
        key_space *element = create_element();
        load_element(tab, element);
        arr[cnt] = create_element();
        init_element(arr[cnt], 0, 0, strdup(element->info), element->release, element->key);
        fseek(tab->file, element->offset, SEEK_SET);
        target = element->offset;
        fread(&ch, sizeof(char), 1, tab->file);
        destroy_element(element);
        cnt++;
    }
    *size = cnt;
    arr = (key_space**)realloc(arr, sizeof(key_space*) * (*size));
    return arr;
}

void create_final_file(table *tab, key_space **arr, int size)
{
    table *tab1 = create_table();
    tab1->filename = tab->filename;
    tab1->size = tab->size;
    tab1->offset = 4;
    tab1->file = fopen(tab1->filename, "w+b");
    fwrite(&(tab1->size), sizeof(int), 1, tab1->file);
    for (int i = 0; i < size; ++i) {
        add_element(tab1, arr[i]->key, arr[i]->info);
    }
    destroy_arr(arr, size);
    fclose(tab1->file);
    destroy_table(tab);
    free(tab1);
}

int get_positive_int(int *info)
{
    int n;
    while ((n = scanf("%d", info)) != 1 || *info <= 0)
    {
        if (n == EOF)
            return eof;

        printf("Enter positive int value!\n");
        scanf("%*c");
    }
    return 1;
}

int get_number_releases(int *release, int cnt)
{
    int n;
    while ((n = scanf("%d", release)) != 1 || *release < 0 || *release > cnt)
    {
        if (n == EOF)
            return eof;

        printf("Enter the coorect number of release!\n");
        scanf("%*c");
    }
    return 1;
}

int get_correct_element(table *tab, int key, char *info)
{
    if (table_is_empty(tab))
        return 1;

    fseek(tab->file, tab->offset, SEEK_SET);
    int target = tab->offset;
    char ch;
    while (!feof(tab->file))
    {
        fseek(tab->file, target, SEEK_SET);
        key_space *element = create_element();
        load_element(tab, element);
        if (element->key == key && strcmp(element->info, info) == 0) {
            destroy_element(element);
            return 0;
        }
        fseek(tab->file, element->offset, SEEK_SET);
        target = element->offset;
        fread(&ch, sizeof(char), 1, tab->file);
        destroy_element(element);
    }
    return 1;
}

int get_correct_info(char **info)
{
    if (*info == NULL)
        return eof;

    if (strlen(*info) == 0)
    {
        while (strlen(*info) == 0)
        {
            printf("Information cannot be empty!\n");
            free(*info);
            *info = readline(">");
        }
    }
    return 1;
}

void destroy_arr(key_space **arr, int size)
{
    for (int i = 0; i < size; ++i) {
        destroy_element(arr[i]);
    }
    free(arr);
}

int find_cnt_release(table *tab, int key)
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
        if (element->key == key)
            ++cnt;

        fseek(tab->file, element->offset, SEEK_SET);
        target = element->offset;
        fread(&ch, sizeof(char), 1, tab->file);
        destroy_element(element);
    }
    return cnt;
}

void print_element(key_space *element)
{
    printf("\t%d. %s -- %d\n", element->key, element->info, element->release);
}

int table_is_empty(table *tab)
{
    int cnt = 0;
    char ch;
    fseek(tab->file, tab->offset, SEEK_SET);
    while (!feof(tab->file))
    {
        fread(&ch, sizeof(char), 1, tab->file);
        cnt++;
        if (cnt > 1)
        {
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
    element->len = 0;
    element->key = 0;
    element->release = 0;
    element->info = NULL;
    return element;
}

int load_element(table *tab, key_space *element)
{
    fread(&(element->offset), sizeof(int), 1, tab->file);
    fread(&(element->len), sizeof(int), 1, tab->file);
    element->info = (char*)malloc(sizeof(char) * (element->len + 1));
    fread(element->info, element->len, sizeof(char), tab->file);
    int pos = ftell(tab->file);
    fread(&(element->release), sizeof(int), 1, tab->file);
    fread(&(element->key), sizeof(int), 1, tab->file);
    element->info[element->len] = '\0';
    return pos;
}

void print_element_in_file(table *tab, key_space *element)
{
    fwrite(&(element->offset), sizeof(int), 1, tab->file);
    fwrite(&(element->len), sizeof(int), 1, tab->file);
    fwrite(element->info, element->len, sizeof(char), tab->file);
    fwrite(&(element->release), sizeof(int), 1, tab->file);
    fwrite(&(element->key), sizeof(int), 1, tab->file);
}

void correct_cnt_release(table *tab, int key, int release)
{
    if (table_is_empty(tab) != 1 && find_cnt_release(tab, key) != 0)
    {
        fseek(tab->file, tab->offset, SEEK_SET);
        int target = tab->offset;
        char ch;
        while (!feof(tab->file))
        {
            fseek(tab->file, target, SEEK_SET);
            key_space *element = create_element();
            int pos = load_element(tab, element);
            if (element->key == key && element->release > release)
            {
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

void init_element(key_space *element, int offset, int len, char *info, int release, int key)
{
    element->offset = offset;
    element->len = len;
    element->info = info;
    element->release = release;
    element->key = key;
}

void destroy_element(key_space *element)
{
    if (element->info != NULL)
        free(element->info);

    free(element);
}

int init_table(char *filename, table *tab)
{
    tab->filename = filename;
    tab->file = fopen(tab->filename, "r+b");
    if (tab->file == NULL)
    {
        int size;
        printf("Enter the table size:\n");
        int check = get_positive_int(&size);
        if (check == eof)
            return eof;

        tab->size = size;
        tab->file = fopen(tab->filename, "w+b");
        fwrite(&(tab->size), sizeof(int), 1, tab->file);
        printf("New file opened!\n");
        return 0;
    }
    fread(&(tab->size), sizeof(int), 1, tab->file);
    printf("Opened an existing file!\n");
    return 1;
}

int hash_function(int key, int size)
{
    int *bytes = (int*)calloc(sizeof(int), sizeof(int));
    int j = 0;
    while (key)
    {
        int ost = key % ONE_BIT;
        key /= ONE_BIT;
        bytes[j] = ost;
        ++j;
    }
    int hash = 0;
    for (int i = 0; i < BYTES_SIZE; ++i) {
        hash = HASH_NUMBER * hash + bytes[i];
    }
    free(bytes);
    return hash % size;
}

