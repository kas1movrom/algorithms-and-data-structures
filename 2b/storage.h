#ifndef STORAGE_H_INCLUDED
#define STORAGE_H_INCLUDED

typedef struct list list;

list *create_out_list();
void push_out_front(list *obj, char data); // dont use
void push_out_back(list *obj, char data);
void print_out(list *obj);
void free_list(list **obj);

#endif //LAB2_STORAGE_H
