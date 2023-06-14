#ifndef CACHE_H_INCLUDED
#define CACHE_H_INCLUDED

typedef struct key_space key_space;
typedef struct table table;

typedef struct cache_item cache_item;
typedef struct cache cache;

cache *create_cache(int size);
int cache_is_empty(cache *tab_cache);
int cache_is_full(cache *tab_cache);
void destroy_cache(cache *tab_cache);
void print_cache(cache *tab_cache);
int equals_cache(cache_item element, char *key, int info, int release);
int cache_delete_contains(cache *tab_cache, key_space *element);
cache_item init_item(char *key, int info, int release, char flag, char search);
int index_of_younger(cache *tab_cache);
int put_in_cache(table *tab, cache *tab_cache, char *key, int info, int cnt, char flag, char search);
void print_cache_item(cache_item element);


#endif
