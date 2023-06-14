#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED

typedef struct key_space key_space;
typedef struct table table;

typedef struct cache_item cache_item;
typedef struct cache cache;

table *create_table();
int destroy_table(table *tab);
int table_is_empty(table *tab, cache *tab_cache);
int add_element(table *tab, cache *tab_cache, char *key, int info);
key_space **search_all(table *tab, cache *tab_cache, char *key, int size);
key_space *search_one(table *tab, char *key, int release, cache *tab_cache);
key_space *delete_one(table *tab, cache *tab_cache, char *key, int release);
key_space **delete_all(table *tab, cache *tab_cache, char *key, int size);
int show_table(table *tab, cache *tab_cache);


#endif
