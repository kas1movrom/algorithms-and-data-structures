#ifndef ITERATOR_H_INCLUDED
#define ITERATOR_H_INCLUDED

typedef struct item item;
typedef struct key_space key_space;

item *item_first(key_space *list);
int item_done(item *element);
item *item_next(item *element);

#endif
