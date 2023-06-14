#ifndef DIALOG_H_INCLUDED
#define DIALOG_H_INCLUDED

typedef struct tree tree;

int dialog_import_from_file(tree *wood);
int dialog_print_tree(tree *wood);
int dialog_print_tree_list(tree *wood);
int dialog_special_search(tree *wood);
int dialog_find_element(tree *wood);
int dialog_detour_tree(tree *wood);
int dialog_delete_element(tree *wood);
int dialog_add(tree *wood);
int dialog(const char *msgs[], int N);
int get_correct_info(char **info);
int get_number_releases(int *release, int cnt);
int get_positive_int(int *key);
char *make_info(char *str);
unsigned int make_key(char *str);
int dialog_dot_file(tree *wood);

#endif
