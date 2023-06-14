#ifndef FUNC_H_INCLUDED
#define FUNC_H_INCLUDED

#include "tree.h"
#include <stdio.h>

knot *create_knot();
node *create_node(char *info, item *list);
bool knot_is_sheet(knot *ptr);
bool knot_full(knot *ptr);
bool add_node(char *info, item *list);
void sort_sheet(knot *sheet);
item *create_item(char *key);
bool add_key(knot *ptr, char *key, char *info);
void tree_split(knot *element, int pos);
bool insert_nonfull(knot *ptr, char *key, char *info);
void print_item(item *element);
int print_detour(knot *ptr, char *key);
item *min_search(knot *ptr);
item *max_search(knot *ptr);
int generate_val(char *str);
void move_pointer(knot *ptr, item *element);
void destroy_knot(knot *ptr);
void process_tree(tree *wood);
int choose_index(knot *ptr, char *key);
int get_releases(item *list);
bool knot_contains(knot *ptr, char *key);
void remove_item(knot *ptr, int index);
bool contains_ext(knot *ptr, int index);
bool neigh_contains(knot *ptr, int index);
knot *process1(knot *ptr, int index);
knot *search_previous(knot *ptr, int *index);
knot *search_next(knot *ptr, int *index);
knot *process2(knot *ptr, int index);
char *make_print(knot *ptr, char *result, char *prefix, char *children_prefix, int *len);
char *get_line(FILE *file);
char *make_shield(char *str);
int delete_item(item *element);
void delete_knot(knot *ptr);
char *create_digraph_item(knot *ptr);
int create_dot_file(FILE *file, knot *ptr);
int count_number(int value);
char *its(int value);
char *create_info(char *filename, int cnt, int offset);


#endif
