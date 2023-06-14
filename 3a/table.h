//#include <stdio.h>
#include <stdio.h>
#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED

typedef struct help help;
typedef struct item item;
typedef struct key_space key_space;
typedef struct table table;

char *scan(FILE *fd);
help *make_struct(char *line);
key_space *add_element(table *tab, char *key, unsigned int info);
int destroy_table(table **tab);
table *delete_all(table *tab, char *key);
table *delete_this(table *tab, char *key, unsigned int release);
table *search_all(table *tab, char *key);
table *search_this(table *tab, char *key, unsigned int release);
void print_table(table *tab);
char *enter_string(FILE *filename);

#endif
