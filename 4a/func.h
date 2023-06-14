#ifndef FUNC_H_INCLUDED
#define FUNC_H_INCLUDED

typedef struct knot knot;
typedef struct tree tree;

knot *create_element(unsigned int key, char *info);
void destroy_element(knot *element);
int delete_tree(knot *ptr);
int get_cnt_key(knot *ptr, unsigned int key);
int check_for_det(int key);
unsigned int find_min(knot *ptr);
knot *find_next_element(knot *this);
void set_detour_tree(tree *wood);
void test_detour(tree *wood);
int find_this_release(tree *wood, knot *element);

#endif
