#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED

typedef struct table table;
typedef struct key_space key_space;

table *create_table();
int destroy_table(table *tab);
key_space **delete_all(table *tab, int key, int *size);
key_space *delete_one(table *tab, int key, int release);
int add_element(table *tab, int key, char *info);
key_space *search_one(table *tab, int key, int release);
key_space **search_all(table *tab, int key, int *size);
int table_clean(table *tab);
int show_table(table *tab, int hash);

#endif
