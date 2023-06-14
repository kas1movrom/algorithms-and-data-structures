#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#include "cache.h"
#include "func.h"

int dialog_cache(const char *msgs[], int N)
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

int dialog_cache_add(tree *wood, cache *tab)
{
        char *key;
        char *info;
        printf("Enter element's key:\n");
        key = readline(">");
        printf("Enter element information:\n");
        info = readline(">");

        /*int index = cache_insert(tab, key, info, 1);
        if (index == -1)
        {
        	index = preempt_record(tab, wood);
        	put_in_cache(tab, key, info, 1, index);
        }*/

		insert_tree(wood, key, info);
		int index = cache_insert(tab, key, info, 2);
		if (index == -1)
		{
			index = preempt_record(tab, wood);
			put_in_cache(tab, key, info, 2, index);
		}
        
        printf("Recording was successfully\n");
        free(key);
        free(info);
        return 1;
}

int dialog_cache_delete_element(tree *wood, cache *tab)
{
	char *key;
	printf("Enter the key of the element you are looking for delete:\n");
	key = readline(">");	/*item *element = find_with_cache(wood, tab, key, &flag);
	if (element == NULL) {
		printf("No elements with that key!\n");
	}*/

	/*if (flag) {
		remove_with_cache(tab, key);
		free(element->head->info);
		element->head->next = NULL;
		free(element->head);
		free(element->key);
		free(element);
	} else {
		 char *info = element->head->info;
		 int index = cache_insert(tab, key, info, 0);
		 if (index == -1)
		 {
		 	index = preempt_record(tab, wood);
		 	put_in_cache(tab, key, info, 0, index);
		 }
	}*/

	item *element = tree_search(wood->root, key);
	if (element == NULL) {
		printf("No elements with that key\n");
		return 1;
	}

	//char *info = strdup(element->info);

	remove_tree(wood, key);
	for (int i = 0; i < tab->capacity; ++i){
		if (tab->arr[i].key != NULL && strcmp(tab->arr[i].key, key) == 0) {
			remove_with_cache(tab, key);
		}
	}
	free(key);
	return 1;
}

int dialog_cache_detour_tree(tree *wood, cache *tab)
{
	if (tree_is_empty(wood)) {
		printf("Tree is empty, detour not possible!\n");
		return tab->capacity;
	}
	char *key = readline(">");
	printf("Detour:\n");
	detour_cache_tree(wood, key, tab);
	free(key);
	return 1;
}

int dialog_cache_find_element(tree *wood, cache *tab)
{
	char *key;
	printf("Enter the key of the element you are looking for:\n");
	key = readline(">");
	char flag = 0;
	item *element = find_with_cache(wood, tab, key, &flag);
	if (element == NULL) {
		printf("No elements with that key!\n");
	}

	printf("That element was finded:\n");
	if (flag) {
		print_item(element);
		free(element->head->info);
		element->head->next = NULL;
		free(element->head);
		free(element->key);
		free(element);
	} else {
		print_item(element);
		char *info = element->head->info;
		int index = cache_insert(tab, key, info, 2);
		if (index == -1)
		{
			index = preempt_record(tab, wood);
			put_in_cache(tab, key, info, 2, index);
		}
	}
	free(key);
}

int dialog_cache_print_tree(tree *wood, cache *tab)
{
	printf("Your tree:\n");
	print_tree(wood);
	print_cache(tab);
	return 1;
}


