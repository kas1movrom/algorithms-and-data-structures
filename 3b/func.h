#ifndef FUNC_H_INCLUDED
#define FUNC_H_INCLUDED

typedef struct key_space key_space;
typedef struct table table;
typedef struct cache_item cache_item;
typedef struct cache cache;
typedef char bool;

int init_table(char *filename, table *tab);
int file_is_empty(table *tab);
key_space *create_element();
int load_element(table *tab, key_space *element);
void destroy_element(key_space *element);
void print_element_in_file(table *tab, key_space *element);
void init_element(key_space *element, int offset, int len, int info, int release, char *key);
void print_element(key_space *element);
int add_element_file(table *tab, char *key, int info, int release);
int find_cnt_release(table *tab, char *key, cache *tab_cache);
key_space *delete_one_file(table *tab, char *key, int release);
void destroy_array(key_space **arr, int size);
void correct_cnt_release(table *tab, cache *tab_cache, char *key, int release);
key_space **load_in_file(table *tab, cache *tab_cache, int *size);
void create_final_file(table *tab, key_space **arr, int size);


#endif
