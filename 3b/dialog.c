#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
#include "cache.h"
#include "func.h"
#include <readline/readline.h>
#define eof -1
#define end_session 5

typedef struct table {
	char *filename;
	FILE *file;
	int offset;
} table;

typedef struct key_space {
	int offset;
	int len;
	int info;
	int release;
	char *key;
} key_space;

typedef struct cache {
	cache_item *arr;
	int size;
	int top;
} cache;

int get_positive_int(int *info)
{
        int n;
        while ((n = scanf("%d", info)) != 1 || *info <= 0)
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

int get_correct_key(char **key)
{
        if (*key == NULL)
                return eof;

        if (strlen(*key) == 0)
        {
                while (strlen(*key) == 0)
                {
                        printf("Key cannot be empty!\n");
                        free(*key);
                        *key = readline(">");
                }
        }
        return 1;
}


int dialog(const char *msgs[], int N)
{
    char *errmsg = "";
    int rc;
    int i, n;
    do
    {
                puts(errmsg);
        errmsg = "Choose the correct number of alternatives!";

        for(i = 0; i < N; ++i)
                        puts(msgs[i]);
        puts("Choose showed alternatives -->");
        while ((n = scanf("%d", &rc)) != 1 || rc < 0 || rc > 7)
        {
            if (n == EOF)
            {
                                rc = 0;
                break;
            }
            printf("\n%s\n", errmsg);
            n = scanf("%*c");
         }
         if(n == 0)
            rc = 0;
    } while(rc < 0 || rc >= N);
    return rc;
}

int dialog_add(table *tab, cache *tab_cache)
{
        char *key;
        int info;
        printf("Enter element's key:\n");
        key = readline(">");
        int check_key = get_correct_key(&key);
        if (check_key == eof)
                return eof;

        printf("Enter element information:\n");
        int check = get_positive_int(&info);
        if (check == eof) {
                free(key);
                return eof;
        }
        add_element(tab, tab_cache, key, info);
        free(key);
        return 1;
}

int dialog_find_one(table *tab, cache *tab_cache)
{
        char *key;
        if (table_is_empty(tab, tab_cache)) {
                printf("Table is empty, serach not possible!\n");
                return 0;
        }
        printf("Enter the key of the element ypu are looking for:\n");
        key = readline(">");
        int check_key = get_correct_key(&key);
        if (check_key == eof)
                return eof;

        int cnt = find_cnt_release(tab, key, tab_cache);
        if (cnt == 0) {
                printf("No elements with that key!\n");
                free(key);
                return 0;
        }
        printf("contains %d elements with that key\n", cnt);
        printf("Max number of release == %d\n", cnt - 1);
        printf("Enter number of release, which one would you like to find:\n");
        int release;
        int check = get_number_releases(&release, cnt - 1);
        if (check == eof) {
                free(key);
                return 0;
        }
        key_space *element = search_one(tab, key, release, tab_cache);
        printf("That element was found:\n");
        print_element(element);
        free(key);
        destroy_element(element);
        return 1;
}

int dialog_find_all(table *tab, cache *tab_cache)
{
        char *key;
        if (table_is_empty(tab, tab_cache)) {
                printf("Table is empty, search not possible!\n");
                return 0;
        }
        printf("Enter the key of the element you are looking for:\n");
        key = readline(">");
        int check_key = get_correct_key(&key);
        if (check_key == eof)
                return eof;

        int cnt = find_cnt_release(tab, key, tab_cache);
        if (cnt == 0) {
                printf("No element with that key!\n");
                free(key);
                return 0;
        }
        key_space **arr = search_all(tab, tab_cache, key, cnt);
        printf("That elements were found:\n");
        for (int i = 0; i < cnt; ++i) {
                print_element(arr[i]);
        }
        free(key);
        destroy_array(arr, cnt);
        return 1;
}

int dialog_delete_one(table *tab, cache *tab_cache)
{
        char *key;
        if (table_is_empty(tab, tab_cache)) {
                printf("Table is empty, delete not possible!\n");
                return 0;
        }
        printf("Enter the key of the element you are looking for:\n");
        key = readline(">");
        int check_key = get_correct_key(&key);
        if (check_key == eof)
                return eof;

        int cnt = find_cnt_release(tab, key, tab_cache);
        if (cnt == 0) {
                printf("No element with that key!\n");
                free(key);
                return 0;
        }
        printf("contains %d elements with that key\n", cnt);
        printf("Max number of release == %d\n", cnt - 1);
        printf("Enter number of release, which one would you like to delete:\n");
        int release;
        int check = get_number_releases(&release, cnt - 1);
        if (check == eof) {
                free(key);
                return 0;
        }

        key_space *element = delete_one(tab, tab_cache, key, release);
        printf("That element was deleted:\n");
        print_element(element);
        correct_cnt_release(tab, tab_cache, key, release);
        free(key);
        destroy_element(element);
        return 1;
}

int dialog_delete_all(table *tab, cache *tab_cache)
{
        char *key;
        if (table_is_empty(tab, tab_cache)) {
                printf("Table is empty, delete not possible!\n");
                return 0;
        }
        printf("Enter the key of the element you are looking for:\n");
        key = readline(">");
        int check_key = get_correct_key(&key);
        if (check_key == eof)
                return eof;
        int cnt = find_cnt_release(tab, key, tab_cache);
        if (cnt == 0) {
                printf("No element with that key!\n");
                free(key);
                return 0;
        }
        key_space **arr = delete_all(tab, tab_cache, key, cnt);
        printf("That elements were deleted:\n");
        for (int i = 0; i < cnt; ++i) {
                print_element(arr[i]);
        }
        free(key);
        destroy_array(arr, cnt);
        return 1;
}

int dialog_show_table(table *tab, cache *tab_cache)
{
        if (table_is_empty(tab, tab_cache)) {
                printf("Tbale is empty!\n");
                return 0;
        }
        printf("Your table:\n");
        show_table(tab, tab_cache);
}

int dialog_show_cache(table *tab, cache *tab_cache)
{
        if (cache_is_empty(tab_cache)) {
                printf("Cache is empty!\n");
                return 0;
        }
        printf("Your cache:\n");
        print_cache(tab_cache);
        return tab->offset;
}
