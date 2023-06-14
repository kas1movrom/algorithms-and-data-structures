#include <stdio.h>
#ifndef TABLE_H_ICNLUDED
#define TABLE_H_ICNLUDED

typedef struct table table;
typedef struct item item;
typedef struct key_space key_space;

table *create_table(int size);
int add_item(table *tab, int key, char *info);
int print_table(table *tab);
int destroy_table(table *tab);
item **delete_all(table *tab, unsigned int key, int *size);
key_space *search_all(table *tab, unsigned int key);
key_space *search_one(table *tab, unsigned int key, unsigned int release);
int table_cleaning(table *tab);
char *my_fscanf(FILE *file);
char* make_item(char *str, unsigned int *key);

#endif
