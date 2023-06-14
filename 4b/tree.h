#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#define EXT 2
#include <stdio.h>

typedef struct node {
	char *info;
	struct node  *next;
} node;

typedef struct item {
	char *key;
	node *head;
} item;

typedef struct knot {
	int size;
	item **arr;
	struct knot **child;
	struct knot *parent;
} knot;

typedef struct tree {
	knot *root;
} tree;

typedef char bool;

typedef int (*t_func) (tree *);

bool tree_is_empty(tree *wood);
tree *create_tree();
item *tree_search(knot *element, char *key);
bool insert_tree(tree *wood, char *key, char *info);
int detour_tree(knot *ptr, char *key);
item **special_search(knot *ptr, char *key, int *size);
int remove_release(int release, item *list);
void print_tree(tree *wood);
int remove_tree(tree *wood, char *key);
int delete_tree(knot *ptr);
void dop_process(FILE *file, char *filename, tree *wood);


#endif
