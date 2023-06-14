#ifndef DIALOG_H_INCLUDED
#define DIALOG_H_INCLUDED

typedef struct table table;

int dialog_print_table(table *tab);
int dialog_import(table *tab);
int dialog_clean(table *tab);
int dialog_delete_all(table *tab);
int dialog_find_all(table *tab);
int dialog_find_one(table *tab);
int dialog_add(table *tab);
int dialog(const char *msgs[], int N);


#endif
