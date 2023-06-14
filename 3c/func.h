#ifndef FUNC_H_INCLUDED
#define FUNC_H_INCLUDED

typedef struct item item;
typedef struct key_space key_space;
typedef struct table table;

int get_correct_info(char **info);
int get_number_releases(int *release, int cnt);
int get_positive_int(int *key);
int element_uncorrect(table *tab, unsigned int key, char *info);
void push_front_existing(key_space *list, char *info, int release, int key);
int destroy_arr(item **arr, int *size);
void remove_next(item *ptr);
int destroy_list(key_space *list);
void print_list(key_space *list);
int list_is_empty(key_space *list);
void push_front(key_space *list, char *info, int key);
int get_release(key_space *list, unsigned int key);
void init_item(int release, char *info, int key, item *next, item *element);
item *create_item();
unsigned int hash_function(int key, table *tab);
int table_is_full(table *tab);
int table_is_empty(table *tab);

#endif
