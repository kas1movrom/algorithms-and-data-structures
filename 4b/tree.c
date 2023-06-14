#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#include "func.h"

#define separ " "

bool tree_is_empty(tree *wood)
{
	if (wood->root == NULL || wood->root->size == 0)
	{
		return 1;
	}
	return 0;
}

tree *create_tree()
{
	tree *wood = (tree*)malloc(sizeof(tree));
	wood->root = create_knot();
	return wood;
}

item *tree_search(knot *element, char *key)
{
	if (element == NULL || element->size == 0 || key == NULL) 
	{
		return NULL;
	}

	for (int i = 0; i < element->size; ++i) {
		if (strcmp(element->arr[i]->key, key) == 0) 
		{
			return element->arr[i];
		}
	}

	if (strcmp(key, element->arr[0]->key) < 0) 
	{
		return tree_search(element->child[0], key);
	}
	if (strcmp(key, element->arr[element->size - 1]->key) > 0)
	{
		return tree_search(element->child[element->size], key);
	}

	for (int i = 0; i < element->size - 1; ++i) {
		if (strcmp(key, element->arr[i]->key) > 0 && strcmp(key, element->arr[i + 1]->key) < 0)
		{
			return tree_search(element->child[i + 1], key);
		}
	}
	return NULL;
}

bool insert_tree(tree *wood, char *key, char *info)
{
	item *list = tree_search(wood->root, key);
	
	if (list != NULL && list->key != NULL && list->head != NULL)
	{
		add_node(info, list);
		return 1;
	}

	knot *root = wood->root;
	if (knot_full(root))
	{
		knot *element = create_knot();
		element->child[0] = root;
		wood->root = element;
		tree_split(element, 0);
		root = element;
	}
	insert_nonfull(root, key, info);

	return 1;
	
}

int detour_tree(knot *ptr, char *key)
{
	if (ptr == NULL)
	{
		return 1;
	}	
	print_detour(ptr, key);
	for (int i = 0; i < 2 * EXT; ++i) {
		detour_tree(ptr->child[i], key);
	}
	return 1;
}

item **special_search(knot *ptr, char *key, int *size)
{
	item *max = max_search(ptr);
	item *min = min_search(ptr);
	item **arr = (item**)malloc(sizeof(item*) * 2);
	*size = 2;

	int min_val = generate_val(min->key), max_val = generate_val(max->key), 
		cur = generate_val(key);

	int koeffs[2] = {abs(cur - min_val), abs(cur - max_val)};
	printf("koeff_for_min:%d, koeff_for_max:%d\n", koeffs[0], koeffs[1]);

	if (koeffs[0] == koeffs[1] && koeffs[0] != 0)
	{
		arr[0] = max;
		arr[1] = min;
		return arr;
	}

	*size = 1;
	arr = (item**)realloc(arr, sizeof(item*));
	if (koeffs[0] > koeffs[1]) {
		arr[0] = min;
	} else {
		arr[0] = max;
	}

	if (strcmp(arr[0]->key, key) != 0)
	{
		return arr;
	}
	*size = 0;
	arr[0] = NULL;
	free(arr);
	return NULL;
}

int remove_release(int release, item *list)
{
	const int cnt = get_releases(list); 
	int cnt1 = cnt, cnt2 = cnt;
	bool flag = cnt == release;
	node *result = list->head;
	while (cnt1 != release)
	{
		--cnt1;
		result = result->next;
	}
	
	if (flag) {
		list->head = result->next;
	} else {
		node *ptr = list->head;
		while (cnt2 != release + 1)
		{
			--cnt2;
			ptr = ptr->next;
		}
		ptr->next = result->next;
	}
	free(result->info);
	free(result);
	return 1;
}

void print_tree(tree *wood)
{
	char *result = (char*)malloc(sizeof(char) * 15000);
	int len = 0;
	knot *element = create_knot();
	element->child[0] = wood->root;
	element->size = 1;
	result = make_print(element, result, "", "", &len);
	result = (char*)realloc(result, sizeof(char) * (len + 1));
	result[len] = '\0';
	element->child[0] = NULL;
	destroy_knot(element);
	printf("%s\n", result);
	free(result);
}

int remove_tree(tree *wood, char *key)
{
	process_tree(wood);
	knot *ptr = wood->root;
	while (!knot_is_sheet(ptr))
	{
		int index = choose_index(ptr, key);
		printf("\nindex:%d\n", index);
		if (index != 2 * EXT - 1 && ptr->arr[index] != NULL && strcmp(key, ptr->arr[index]->key) == 0) {
			ptr = process2(ptr, index);
		} else {
			printf("\nchoose situation1\n");
			ptr = process1(ptr, index);
		}
	}
	int index = knot_contains(ptr, key);
	if (index != -1)
	{
		remove_item(ptr, index);
		return 1;
	}
	return 0;
}

int delete_tree(knot *ptr)
{
	if (ptr == NULL) {
		return 1;
	}
	for (int i = 0; i < ptr->size + 1; ++i) {
		delete_tree(ptr->child[i]);
	}
	delete_knot(ptr);
	destroy_knot(ptr);
	return 1;
}

void dop_process(FILE *file, char *filename, tree *wood)
{
	int cnt = 0;
	char *line = get_line(file); 
	while (line != NULL)
	{
		int offset = 0, len = 0;
		char *token = strtok(line, separ);
		while (token != NULL)
		{
			len = strlen(token);
			char *info = create_info(filename, cnt, offset);
			offset += (len + 1);
			insert_tree(wood, token, info);
			free(info);
			token = strtok(NULL, separ);
		}
		++cnt;
		free(line);
		line = get_line(file);
	}
}


