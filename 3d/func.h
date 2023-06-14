#ifndef FUNC_H_INCLUDED
#define FUNC_H_INCLUDED

typedef struct table table;
typedef struct key_space key_space;

int get_correct_info(char **info);
int get_correct_element(table *tab, int key, char *info);
int get_number_releases(int *release, int cnt);
int get_positive_int(int *info);
void create_final_file(table *tab, key_space **arr, int size);
key_space **load_in_file(table *tab, int *size);
void destroy_arr(key_space **arr, int size);
int find_cnt_release(table *tab, int key);
void print_element(key_space *element);
void destroy_element(key_space *element);
void init_element(key_space *element, int offset, int len, char *info, int release, int key);
void correct_cnt_release(table *tab, int key, int release);
void print_element_in_file(table *tab, key_space *element);
int load_element(table *tab, key_space *element);
key_space *create_element();
int table_is_empty(table *tab);
int init_table(char *filename, table *tab);
int hash_function(int key, int size);

#endif
