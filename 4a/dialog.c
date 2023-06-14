#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include "dialog.h"
#include "func.h"
#include "tree.h"

typedef struct knot {
	unsigned int key;
	char *info;

	struct knot *right;
	struct knot *left;
	struct knot *parent;
	struct knot *detour;
} knot;

typedef struct tree {
	knot *root;
} tree;

#define SUCCESS 1
#define FAIL 0

#define eof -1

unsigned int make_key(char *str)
{
    return atoi(str);
}

char *make_info(char *str)
{
    return strdup(str);
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
        while ((n = scanf("%d", &rc)) != 1 || rc < 0 || rc > 9)
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


int dialog_add(tree *wood)
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

        add_knot(wood, create_element(key, info));
        printf("Element was successfully written to the tree\n");
        free(info);
        set_detour_tree(wood);
        return SUCCESS;;
}

int dialog_delete_element(tree *wood)
{
        if (tree_is_empty(wood)) {
                printf("Tree is empty, delete not possible!\n");
                return FAIL;
        } else {
                int key;
                printf("Enter the key of the element you are looking for delete:\n");
                int check = get_positive_int(&key);
                if (check == eof)
                        return eof;

                int cnt = get_cnt_key(wood->root, key);
                if (cnt == 0) {
                        printf("No elements with that key!\n");
                        return FAIL;
                }
                knot *element = delete_element(wood, find_knot(wood->root, key, 0));
                printf("That element was deleted: %d. %s", element->key, element->info);
                destroy_element(element);
                set_detour_tree(wood);
                return SUCCESS;
        }
}

int dialog_detour_tree(tree *wood)
{
        if (tree_is_empty(wood)) {
                printf("Tree is empty, detour not possible!\n");
                return FAIL;
        }
        int cnt;
        printf("Enter number of digits:\n");
        int check = get_positive_int(&cnt);
        if (check == eof)
                return eof;

        printf("Detour:\n");
        detour_tree(wood->root, cnt);
        return SUCCESS;
}

int dialog_find_element(tree *wood)
{
        if (tree_is_empty(wood)) {
                printf("Tree is empty, search not possible!\n");
                return FAIL;
        }
        int key;
        printf("Enter the key of the element you are looking for:\n");
        int check = get_positive_int(&key);
        if (check == eof)
                return eof;

        int cnt = get_cnt_key(wood->root, key);
        if (cnt == 0) {
                printf("No elements with that key!\n");
                return FAIL;
        }
        printf("Contains %d elements with that key\n", cnt);
        printf("Max number of release == %d\n", cnt - 1);
        printf("Enter number of release, which one would you like to find:\n");
        int release;
        int check_rel = get_number_releases(&release, cnt - 1);
        if (check_rel == eof)
                return eof;

        knot *element = find_knot(wood->root, key, release);
        printf("that element was found:\n");
        printf("%d. %s\n", element->key, element->info);
        return SUCCESS;
}

int dialog_special_search(tree *wood)
{
        if (tree_is_empty(wood)) {
                printf("Tree is empty, search not possible!\n");
                return FAIL;
        }
        int key = find_min(wood->root);
        int cnt = get_cnt_key(wood->root, key);
        printf("Contains %d elements with that key\n", cnt);
        printf("Max number of release == %d\n", cnt - 1);
        printf("Enter number of release, which one would you like to find:\n");
        int release;
        int check_rel = get_number_releases(&release, cnt - 1);
        if (check_rel == eof)
                return eof;

        knot *element = find_knot(wood->root, key, release);
        printf("that element was found:\n");
        printf("%d. %s\n", element->key, element->info);
        return SUCCESS;
}

int dialog_print_tree_list(tree *wood)
{
        if (tree_is_empty(wood)) {
                printf("Tree is empty!\n");
                return FAIL;
        }
        printf("Your tree:\n");
        print_tree(wood->root);
        return SUCCESS;
}

int dialog_print_tree(tree *wood)
{
        if (tree_is_empty(wood)) {
                printf("Tree is empty\n");
        }
        printf("Your tree:\n");
        printf_tree(wood->root, 0);
        return SUCCESS;
}

int dialog_import_from_file(tree *wood)
{
        char *filename = readline(">");
        if (get_correct_info(&filename) == eof)
                return eof;

        FILE *file = fopen(filename, "r");
        if (file == NULL)
        {
                printf("Error with the file opening!\n");
                free(filename);
                return FAIL;
        }
        int cnt = 0;
        char *line = get_line(file);
        unsigned int key;
        while (line != NULL)
        {
                if (cnt % 2 == 0) {
                        key = make_key(line);
                } else {
                        char *info = make_info(line);
                        add_knot(wood, create_element(key, info));
                        free(info);
                }
                free(line);
                cnt++;
                line = get_line(file);
        }
        fclose(file);
        printf("Recording was successfull!\n");
        free(filename);
        set_detour_tree(wood);
        return SUCCESS;
}

int dialog_dot_file(tree *wood)
{
	if (tree_is_empty(wood)) {
		printf("Tree is empty!\n");
		return FAIL;
	}
	printf("Enter the file name:\n");
	//char *filename = readline(">");
	char *filename = "file.dot";
	//if (get_correct_info(&filename) == eof)
		//return eof;

	FILE *file = fopen(filename, "w");
	fprintf(file, "digraph {\n");
	create_dot_file(file, wood->root, wood);
	fprintf(file, "}");
	fclose(file);
	system("dot -Tpng file.dot -o result.png");
	//free(filename);
	return SUCCESS;
}
