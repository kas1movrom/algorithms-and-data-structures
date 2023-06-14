#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "dialog.h"
#include "func.h"
#include "table.h"

#define eof -1

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

int dialog_print_table(table *tab)
{
        if (table_is_empty(tab))
        {
                printf("Table is empty!\n");
                return 0;
        }
        printf("Your table:\n");
        print_table(tab);
        return 1;
}

int dialog_import(table *tab)
{
        if (!table_is_empty(tab)) {
                printf("Table is not empty, import not possible!\n");
                return 0;
        }
        char *filename = readline(">");
        if (get_correct_info(&filename) == eof)
                return eof;

        FILE *file = fopen(filename, "r");
        if (file == NULL) {
                printf("Error wit the file opening!\n");
                free(filename);
                return 0;
        }
        char *line = my_fscanf(file);
        int cnt = 0;
        char **arr = (char**)malloc(sizeof(char*) * 100);
        while (line != NULL)
        {
                arr[cnt] = line;
                ++cnt;
                line = my_fscanf(file);
        }
        if (cnt == 0) {
                printf("File is empty!\n");
                free(arr);
                free(filename);
                fclose(file);
                return 0;
        }
        arr = (char**)realloc(arr, sizeof(char*) * cnt);
        fclose(file);

        unsigned int key;
        for (int i = 0; i < cnt; ++i) {
                char *info = make_item(arr[i], &key);
                add_item(tab, key, info);
                free(info);
        }
        free(filename);
        for (int i = 0; i < cnt; ++i) {
                free(arr[i]);
        }
        free(arr);
        printf("Recording was successfull!\n");
        return 1;
}

int dialog_clean(table *tab)
{
        if (table_is_empty(tab)) {
                printf("Table is empty, cleaning not possible!\n");
                return 0;
        }
        int cnt = table_cleaning(tab);
        if (cnt == 0) {
                printf("cleaning was successful, but nothing has changed!\n");
                return 0;
        }
        printf("Cleaning was successfull!\n");
        return 1;
}

int dialog_delete_all(table *tab)
{
        if (table_is_empty(tab)) {
                printf("Table is empty, delete not possible!\n");
                return 0;
        } else {
                int key;
                printf("Enter the key of the elements you are looking for delete:\n");
                int check = get_positive_int(&key);
                if (check == eof)
                        return eof;

                int cnt = get_release(tab->arr[hash_function(key, tab)], key);
                if (cnt == 0) {
                        printf("No elements with that key!\n");
                        return 0;
                } else {
                        int size = 0;
                        item **arr = delete_all(tab, key, &size);
                        printf("That elements were deleted:\n");
                        for (int i = 0; i < size; ++i) {
                                printf("\t%d. %s -- %d\n", arr[i]->key, arr[i]->info, arr[i]->release);
                        }
                        destroy_arr(arr, &size);
                        return 1;
                }
        }
}

int dialog_find_all(table *tab)
{
        if (table_is_empty(tab)) {
                printf("table is empty, search not possible!\n");
                return 0;
        } else {
                int key;
                printf("Enter the key of the elements you are looking for:\n");
                int check = get_positive_int(&key);
                if (check == eof)
                        return eof;

                int cnt = get_release(tab->arr[hash_function(key, tab)], key);
                if (cnt == 0) {
                        printf("No elements with that key!\n");
                        return 0;
                } else {
                        key_space *list = search_all(tab, key);
                        printf("That elements was found:\n");
                        print_list(list);
                        destroy_list(list);
                        free(list);
                        return 1;
                }
        }
}

int dialog_find_one(table *tab)
{
        if (table_is_empty(tab)) {
                printf("table is empty, search not possible!\n");
                return 0;
        } else {
                int key;
                printf("Enter the key of the element you are looking for:\n");
                int check = get_positive_int(&key);
                if (check == eof)
                        return eof;

                int cnt = get_release(tab->arr[hash_function(key, tab)], key);
                if (cnt == 0) {
                        printf("No element with that key!\n");
                        return 0;
                } else {
                        printf("Contains %d elements with that key\n", cnt);
                        printf("Max number of release == %d\n", cnt - 1);
                        printf("Enter number of release. which one would you like to find:\n");
                        int release;
                        int check_rel = get_number_releases(&release, cnt - 1);
                        if (check_rel == eof)
                                return eof;

                        key_space *list = search_one(tab, key, release);
                        printf("That element was found:\n");
                        print_list(list);
                        destroy_list(list);
                        free(list);
                        return 1;
                }
        }
}

int dialog_add(table *tab)
{
        char *info;
        int key;
        printf("Enter element key:\n");
        int check = get_positive_int(&key);
        if (check == eof)
                return eof;

        printf("Enter elementt info:\n");
        info = readline(">");
        int check_info = get_correct_info(&info);
        if (check_info == eof)
                return eof;

        if (element_uncorrect(tab, key, info)) {
                free(info);
                printf("There is a such an element!\n");
                return 0;
        }
        add_item(tab, key, info);
        printf("Element was successfully written to the table\n");
        free(info);
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
