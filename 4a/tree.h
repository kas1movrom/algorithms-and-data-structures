#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED
#include <stdio.h>

typedef struct tree tree;
typedef struct knot knot;

int tree_is_empty(tree *wood);
tree *create_tree();
void destroy_tree(tree *wood);
int add_knot(tree *wood, knot *element);
int print_tree(knot *ptr);
int detour_tree(knot *ptr, int cnt);
knot *find_knot(knot *ptr, unsigned int key, int release);
knot *delete_element(tree *wood, knot *this);
void printf_tree(knot *element, int p);
char *get_line(FILE *file);
int create_dot_file(FILE *file, knot *ptr, tree *wood);

#endif
