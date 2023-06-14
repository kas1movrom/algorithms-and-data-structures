#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#include "cache.h"
#include "func.h"

unsigned long long hash(char *key)
{
	unsigned long long hash = 0;
	for (int i = 0; i < (int)strlen(key); ++i)
	{
		hash = 37 * hash + (int)key[i];
	}
	return hash;
}

char coprime(int numb, int capacity)
{
	int cnt = 0;
	for (int i = 1; i <= numb; ++i) {
		if (numb % i == 0 && capacity % i == 0)
		{
			++cnt;
		}	
	}
	return cnt == 1;
}

int find_step(int capacity)
{
	for (int i = 2; i < capacity; ++i) {
		if (coprime(i, capacity)) 
		{
			return i;
		}
	}
}

void fill_cache(cache *tree_cache)
{
	for (int i = 0; i < tree_cache->capacity; ++i) {
		tree_cache->arr[i].key = NULL;
		tree_cache->arr[i].info = NULL;
		tree_cache->arr[i].busy = 0;
	}
}

cache *create_cache(int capacity)
{
	cache *tree_cache = (cache*)malloc(sizeof(cache));
	tree_cache->capacity = capacity;
	tree_cache->step = find_step(capacity);
	tree_cache->arr = (cache_item*)malloc(sizeof(cache_item) * capacity);
	fill_cache(tree_cache);
	return tree_cache;
}

int put_in_cache(cache *tab, char *key, char *info, char flag, int index)
{
	tab->arr[index].busy = 1;
	tab->arr[index].key = strdup(key);
	tab->arr[index].info = strdup(info);
	tab->arr[index].age = 0;
	switch(flag)
	{
		case 0 :
			tab->arr[index].deleted = 1;
			tab->arr[index].recorded = 0;
			break;

		case 1 :
			tab->arr[index].deleted = 0;
			tab->arr[index].recorded = 1;
			break;

		default :
			tab->arr[index].recorded = 0;
			tab->arr[index].deleted = 0;
	}
	return 1;
}

int cache_insert(cache *tab, char *key, char *info, char flag)
{
	int index = hash(key) % tab->capacity, cnt = 0;
	while (tab->arr[index].busy == 1 && cnt < tab->capacity)
	{
		//++tab->arr[index].age;
		index = (index + tab->step) % tab->capacity;
		++cnt;
	}
	if (cnt < tab->capacity)
	{
		put_in_cache(tab, key, info, flag, index);
		return index;
	}
	return -1;
}

int find_younger(cache *tab)
{
	int index = 0, cnt = 0, age = tab->arr[index].age, min_id = 0;
	while (tab->arr[index].busy && cnt < tab->capacity)
	{
		if (tab->arr[index].age < age)
		{
			min_id = index;
			age = tab->arr[index].age;
		}
		index = (index + tab->step) % tab->capacity;
		++cnt;
	}
	return min_id;
}

void reset_item(cache *tab, int index)
{
	tab->arr[index].key = NULL;
	tab->arr[index].info = NULL;
	tab->arr[index].busy = 0;
	tab->arr[index].age = 0;
}

void pre_insert(tree *wood, cache *tab, int index)
{
	insert_tree(wood, tab->arr[index].key, tab->arr[index].info);
	free(tab->arr[index].key);
	free(tab->arr[index].info);
	reset_item(tab, index);
}

void pre_remove(tree *wood, cache *tab, int index)
{
	remove_tree(wood, tab->arr[index].key);
	free(tab->arr[index].key);
	free(tab->arr[index].info);
	reset_item(tab, index);
}

void pre_search(cache *tab, int index)
{
	free(tab->arr[index].key);
	free(tab->arr[index].info);
	reset_item(tab, index);
}

int preempt_record(cache *tab, tree *wood)
{
	int index = find_younger(tab);
	if (tab->arr[index].recorded) {
	
		pre_insert(wood, tab, index);
		
	} else if (tab->arr[index].deleted) {

		pre_remove(wood, tab, index);
				
	} else {

		pre_search(tab, index);
		
	}
	return index;
}

void print_cache_item(cache_item element)
{
	printf("%d, %s -- %s | %d | %d | %d\n", element.busy, element.key, element.info, element.recorded,
		element.deleted, element.age);
}

void print_cache(cache *tab)
{
	int index = 0, cnt = 0;
	while (cnt < tab->capacity)
	{
		if (tab->arr[index].busy)
		{
			++tab->arr[index].age;
			print_cache_item(tab->arr[index]);
		}
		index = (index + tab->step) % tab->capacity;
		++cnt;
	}
}

char cache_delete_contains(cache *tab, char *key)
{
	int index = 0, cnt = 0;
	while (cnt < tab->capacity)
	{
		if (tab->arr[index].busy)
		{
			//++tab->arr[index].age;
			if (strcmp(key, tab->arr[index].key) == 0 && tab->arr[index].deleted)
			{
				return 1;
			}
		}
		index = (index + tab->step) % tab->capacity;
		++cnt;
	}
	return 0;
}

item *find_with_cache(tree *wood, cache *tab, char *key, char *flag)
{
	int index = 0, cnt = 0;
	*flag = 1;
	while (cnt < tab->capacity)
	{
		if (tab->arr[index].busy)
		{
			++tab->arr[index].age;
			if (strcmp(key, tab->arr[index].key) == 0 && (tab->arr[index].recorded ||
				(!tab->arr[index].recorded && !tab->arr[index].deleted))) {
				item *element = create_item(key);
				node *ptr = create_node(tab->arr[index].info, element);
				element->head = ptr;
				return element;
			}
		}
		index = (index + tab->step) % tab->capacity;
		++cnt;
	}

	*flag = 0;
	return tree_search(wood->root, key);
}

int remove_with_cache(cache *tab, char *key)
{
	int index = 0, cnt = 0;
	while (cnt < tab->capacity)
	{
		if (tab->arr[index].busy)
		{
			++tab->arr[index].age;
			if (!strcmp(tab->arr[index].key, key))
			{
				free(tab->arr[index].key);
				free(tab->arr[index].info);
				reset_item(tab, index);
				break;
			}
		}
		index = (index + tab->step) % tab->capacity;
		++cnt;
	}
	return 1;
}

int print_cache_detour(cache *tab, knot *ptr, char *key)
{
	if (key == NULL)
	{
		for (int i = 0; i < ptr->size; ++i) {
			if (!cache_delete_contains(tab, ptr->arr[i]->key)) {
				print_item(ptr->arr[i]);
			}
		}
		return 1;
	}

	for (int i = 0; i < ptr->size; ++i) {
		if (strcmp(ptr->arr[i]->key, key) < 0 && !cache_delete_contains(tab, ptr->arr[i]->key)) {
			print_item(ptr->arr[i]);
		}
	}
}

int detour_cache(cache *tab, knot *ptr, char *key)
{
	if (ptr == NULL)
	{
		return 1;
	}
	print_cache_detour(tab, ptr, key);
	for (int i = 0; i < 4; ++i) {
		detour_cache(tab, ptr->child[i], key);
	}
}

int detour_cache_tree(tree *wood, char *key, cache *tab)
{
	detour_tree(wood->root, key);
	for (int i = 0; i < tab->capacity; ++i) {
		if (tab->arr[i].busy && strcmp(tab->arr[i].key, key) < 0)
		{
			print_cache_item(tab->arr[i]);
		}
	}
	return 1;
}

void free_cache(cache *tab, tree *wood)
{
	for (int i = 0; i < tab->capacity; ++i) {
		if (tab->arr[i].busy)
		{
			if (tab->arr[i].recorded) {
				pre_insert(wood, tab, i);
			} else if (tab->arr[i].deleted) {
				pre_remove(wood, tab, i);
			} else {
				pre_search(tab, i);
			}
		}
	}
	free(tab->arr);
	free(tab);
}
