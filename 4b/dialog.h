#ifndef DIALOG_H_INCLUDED
#define DIALOG_H_INCLUDED
#include "func.h"

int get_positive_int(int *key);
int get_number_releases(int *release, int cnt);
int get_correct_info(char **info);
int dialog(const char *msgs[], int N);
int dialog_add(tree *wood);
int dialog_delete_element(tree *wood);
int dialog_detour_tree(tree *wood);
int dialog_find_element(tree *wood);
int dialog_special_search(tree *wood);
int dialog_print_tree(tree *wood);
int dialog_import_from(tree *wood);
int dialog_dot_file(tree *wood);
int dialog_process_file(tree *wood);



#endif
