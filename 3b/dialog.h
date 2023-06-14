#ifndef DIALOG_H_INCLUDED
#define DIALOG_H_ICNLUDED

typedef struct table table;
typedef struct cache cache;

int get_positive_int(int *info);
int get_number_releases(int *release, int cnt);
int get_correct_key(char **key);
int dialog(const char *msgs[], int N);
int dialog_add(table *tab, cache *tab_cache);
int dialog_find_one(table *tab, cache *tab_cache);
int dialog_find_all(table *tab, cache *tab_cache);
int dialog_delete_one(table *tab, cache *tab_cache);
int dialog_delete_all(table *tab, cache *tab_cache);
int dialog_show_table(table *tab, cache *tab_cache);
int dialog_show_cache(table *tab, cache *tab_cache);

#endif
