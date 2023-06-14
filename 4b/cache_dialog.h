#ifndef CACHE_DIALOG_H_INCLUDED
#define CACHE_DIALOG_H_INCLUDEDA

#include "func.h"
#include "cache.h"

int dialog_cache(const char *msgs[], int N);
int dialog_cache_add(tree *wood, cache *tab);
int dialog_cache_delete_element(tree *wood, cache *tab);
int dialog_cache_detour_tree(tree *wood, cache *tab);
int dialog_cache_find_element(tree *wood, cache *tab);
int dialog_cache_print_tree(tree *wood, cache *tab);

#endif
