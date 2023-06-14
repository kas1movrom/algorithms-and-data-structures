#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <readline/readline.h>

//#include "tree.h"
#include "func.h"


int get_positive_int(int *key)
{
    int n;
    while ((n = scanf("%d", key)) != 1 || *key <= 0)
    {
        if (n == EOF)
        {
                return -1;
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
                return -1;
        }
        printf("Enter the correct number of release!\n");
        scanf("%*c");
    }
    return 1;
}

int get_correct_info(char **info)
{
    if (*info == NULL)
        return -1;

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
        while ((n = scanf("%d", &rc)) != 1 || rc < 0 || rc > 10)
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
	char *key, *info;
	printf("Enter element key:\n");
	key = readline(">");
	int check = get_correct_info(&key);
	if (check == -1) {
		return -1;
	}

	printf("Enter element info;\n");
	info = readline(">");
	int check_i = get_correct_info(&info);
	if (check_i == -1) {
		return -1;
	}

	insert_tree(wood, key, info);
	printf("Element was successfully written to the tree\n");
	free(key);
	free(info);
	return 1; 
}

int dialog_delete_element(tree *wood)
{
	if (tree_is_empty(wood)) {
		printf("Tree is empty, delete not possible!\n");
		return 0;
	}
	char *key;
	printf("Enter the key of the element you are looking for delete");
	key = readline(">");
	int check = get_correct_info(&key);
	if (check == -1) {
		return -1;
	}

	item *element = tree_search(wood->root, key);
	if (element == NULL) {
		free(key);
		printf("There is no elements with that key!\n");
		return 0;
	}
	int cnt = get_releases(element);
	if (cnt == 1) {
		remove_tree(wood, key);
		free(key);
		printf("Element was successfully delete!\n");
		return 1;	
	} 

	printf("Contains %d informatiob with that key\n", cnt);
	printf("Max number of release == %d\n", cnt);
	printf("Enter number of release, which one would you like to delete:\n");
	int release;
	int check_re = get_number_releases(&release, cnt);
	if (check_re == -1) {
		free(key);
		return -1;
	}

	remove_release(release, element);
	printf("element was successfuly delete\n");
	free(key);
	element = NULL;
	return 1;
}


int dialog_detour_tree(tree *wood)
{
	if (tree_is_empty(wood)) {
		printf("Tree is empty, detour not possible!\n");
		return 0;
	}
	char *key = readline(">");
	get_correct_info(&key);
	printf("Detour:\n");
	detour_tree(wood->root, key);
	free(key);
	return 1;
}

int dialog_find_element(tree *wood)
{
	if (tree_is_empty(wood)) {
		printf("Tree is empty, search not possible!\n");
		return 0;
	}

	char *key;
	printf("Enter the key of the element you are looking for:\n");
	key = readline(">");
	int check = get_correct_info(&key);
	if (check == -1) {
		return -1;
	}

	item *element = tree_search(wood->root, key);
	if (element == NULL) {
		printf("No elements with that key!\n");
		free(key);
		return 0;
	}

	printf("That elements was found:\n");
	print_item(element);
	free(key);
	return 1;
}

int dialog_special_search(tree *wood)
{
	if (tree_is_empty(wood)) {
		printf("Tree is empty, search not possible!\n");
		return 0;
	}
	printf("Enter the key\n");
	char *key = readline(">");
	int check = get_correct_info(&key);
	if (check == -1) {
		return -1;
	}

	int size = 0;
	item **arr = special_search(wood->root, key, &size);
	if (!size) {
		printf("There are no elements different from this one\n");
		free(key);
		return 0;
	}

	printf("That elements were found:\n");
	for (int i = 0; i < size; ++i) {
		print_item(arr[i]);
		arr[i] = NULL;
	}	
	free(key);
	free(arr);
	return 1;
}

int dialog_print_tree(tree *wood)
{
	if (tree_is_empty(wood)) {
		printf("Tree is empty!\n");
	}
	printf("Your tree:\n");
	print_tree(wood);
	return 1;
}

int dialog_import_from(tree *wood)
{
	printf("Enter the file name:\n");
	char *filename = readline(">");
	if (get_correct_info(&filename) == -1) {
		return -1;
	}

	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("Error with the file opening!\n");
		free(filename);
		return 0;
	}
	int cnt = 0;
	char *line = get_line(file), *key;
	while (line != NULL)
	{
		if (cnt % 2 == 0) {
			key = make_shield(line);
		} else {
			char *info = make_shield(line);
			insert_tree(wood, key, info);
			free(key);
			free(info);
		}
		free(line);
		++cnt;
		line = get_line(file);
	}
	fclose(file);
	printf("Recording was successfull!\n");
	free(filename);
	return 1;
}

int dialog_dot_file(tree *wood)
{
	if (tree_is_empty(wood)) {
		printf("Tree is empty!\n");
		return 0;
	}
	char *filename = "file.dot";
	FILE *file = fopen(filename, "w");
	fprintf(file, "digraph {\n");
	create_dot_file(file, wood->root);
	fprintf(file, "}");
	fclose(file);
	system("dot -Tpng file.dot -o result.png");
	system("catimg result.png");
	return 1;
}

int dialog_process_file(tree *wood)
{
	printf("Enter the file name:\n");
	char *filename = readline(">");
	if (get_correct_info(&filename) == -1) {
		return -1;
	}

	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("Error with the file opening!\n");
		free(filename);
		return 0;		
	}	
	dop_process(file, filename, wood);
	fclose(file);
	printf("Recording was successfully!\n");
	free(filename);
	return 1;
}
