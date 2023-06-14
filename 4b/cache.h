#ifndef CACHE_H_INCLUDED
#define CACHE_H_INCLUDED

#include "func.h"

typedef struct cache_item {
	char *key;
	char *info;
	int busy;

	char recorded;
	char deleted;
	int age;
} cache_item;

typedef struct cache {
	int capacity;
	int step;
	cache_item *arr;	
} cache;

unsigned long long hash(char *key);
char coprime(int numb, int capacity);
int find_step(int capacity);
void fill_cache(cache *tree_cache);
cache *create_cache(int capacity);
int put_in_cache(cache *tab, char *key, char *info, char flag, int index);
int cache_insert(cache *tab, char *key, char *info, char flag);
int find_younger(cache *tab);
void reset_item(cache *tab, int index);
int preempt_record(cache *tab, tree *wood);
void print_cache_item(cache_item element);
void print_cache(cache *tab);
char cache_delete_contains(cache *tab, char *key);
item *find_with_cache(tree *wood, cache *tab, char *key, char *flag);
int remove_with_cache(cache *tab, char *key);
int detour_cache_tree(tree *wood, char *key, cache *tab);
void free_cache(cache *tab, tree *wood);
void pre_insert(tree *wood, cache *tab, int index);
void pre_remove(tree *wood, cache *tab, int index);
void pre_search(cache *tab, int index);
int print_cache_detour(cache *tab, knot *ptr, char *key);
int detour_cache_tree(tree *wood, char *key, cache *tab);
int detour_cache(cache *tab, knot *ptr, char *key);

#endif
