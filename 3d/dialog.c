#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "table.h"
#include "dialog.h"
#include "func.h"

#define eof -1

typedef struct table {
	char *filename;
	FILE *file;
	int offset;
	int size;
} table;

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

int dialog_add(table *tab)
{
        char *info;
        int key;
        printf("Enter element key:\n");
        int check = get_positive_int(&key);
        if (check == eof)
                return eof;

        printf("Enter element info:\n");
        info = readline(">");
        int check_info = get_correct_info(&info);
        if (check_info == eof)
                return eof;

        if (!get_correct_element(tab, key, info))
        {
                free(info);
                printf("There is a such an element!\n");
                return 0;
        }
        add_element(tab, key, info);
        printf("Element was successfully written to the table\n");
        free(info);
        return 1;
}

int dialog_find_one(table *tab)
{
        if (table_is_empty(tab)) {
                printf("Table is empty! Search not possible!\n");
                return 0;
        } else {
                int key;
                printf("Enter the key of the element you are looking for:\n");
                int check = get_positive_int(&key);
                if (check == eof)
                        return eof;

                int cnt  = find_cnt_release(tab, key);
                if (cnt == 0) {
                        printf("No element with that key!\n");
                        return 0;
                } else {
                        printf("Contains %d elements with that key\n", cnt);
            printf("Max number of release == %d\n", cnt - 1);
            printf("Enter number of release, which one would you like to find:\n");
                        int release;
                        int check_rel = get_number_releases(&release, cnt - 1);
                        if (check_rel == eof)
                                return eof;

                        key_space *element = search_one(tab, key, release);
                        printf("That element was found:\n");
                        print_element(element);
                        destroy_element(element);
                        return 1;
                }
        }
}

int dialog_find_all(table *tab)
{
        if (table_is_empty(tab)) {
                printf("Table is empty, search not possible\n");
                return 0;
        } else {
                int key;
                printf("Enter the key of the elements you are looking for:\n");
                int check = get_positive_int(&key);
                if (check == eof)
                        return eof;

                int cnt = find_cnt_release(tab, key);
                if (cnt == 0) {
                        printf("No elements with that key!\n");
                        return 0;
                } else {
                        int size = 0;
                        key_space **arr = search_all(tab, key, &size);
                        printf("That elements were found:\n");
                        for (int i = 0; i < size; ++i) {
                                print_element(arr[i]);
                        }
                        destroy_arr(arr, size);
                        return 1;
                }
        }
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

                int cnt = find_cnt_release(tab, key);
                if (cnt == 0) {
                        printf("No elements with that key!\n");
                        return 0;
                } else {
                        int size = 0;
                        key_space **arr = delete_all(tab, key, &size);
                        printf("That elements were deleted:\n");
                        for (int i = 0; i < size; ++i) {
                                print_element(arr[i]);
                        }
                        destroy_arr(arr, size);
                        return 1;
                }
        }
}

int dialog_clean(table *tab)
{
        if (table_is_empty(tab)) {
                printf("Table is empty, cleaning not possible!\n");
                return 0;
        }
        int cnt = table_clean(tab);
        if (cnt == 0) {
                printf("Cleaning was successfull, but nothing has changed!\n");
                return 0;
        }
        printf("Cleaning was successfull!\n");
        return 1;
}

int dialog_print_table(table *tab)
{
        if (table_is_empty(tab))
        {
                printf("Table is empty!\n");
                return 0;
        }
        for (int i = 0; i < tab->size; ++i) {
                printf("hash%d#\n", i);
                show_table(tab, i);
        }
}
