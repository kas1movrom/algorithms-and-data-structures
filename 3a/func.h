#ifndef FUNC_H_INCLUDED
#define FUNC_H_INCLUDED

typedef struct item item;
typedef struct key_space key_space;
typedef struct table table;

int table_is_empty(table *tab);
table *create_table();
key_space *create_element(char *key, unsigned int info);
key_space *create_copy(key_space *element);
int equals(key_space *ptr1, key_space *ptr2);
void dias(table *tab, key_space *element);
int cnt_key(table *tab, char *key);
unsigned int add_element_exising(table *tab, key_space *element);
int element_is_here(table *tab, char *key);

key_space *element_first(table *tab);
int element_done(key_space *element);
key_space *element_next(key_space *element);
int element_done_next(key_space *element);

#endif
