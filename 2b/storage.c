#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    char data;
    struct node * next;
    struct node * last;
} node ;

typedef struct list {
    node *head;
    node *tail;
} list;


list *create_out_list() {
    list *obj1 = (list*)malloc(sizeof(list));
    obj1->head = NULL;
    obj1->tail = NULL;
    return obj1;
}

void push_out_front(list *obj, char data) {
    node *ptr = (node*)malloc(sizeof(node));
    ptr->data = data;
    node *point = obj->head;
    if (obj->head == NULL) {
        obj->tail = ptr;
        obj->head = ptr;
        ptr->next = NULL;
        ptr->last = NULL;
    } else {
        ptr->next = point;
        point->last = ptr;
        ptr->last = NULL;
        obj->head = ptr;
    }
}

void push_out_back(list *obj, char data) {
    node *ptr = (node*)malloc(sizeof(node));
    ptr->data = data;
    node *point = obj->tail;
    if (obj->tail == NULL) {
        obj->tail = ptr;
        obj->head = ptr;
        ptr->next = NULL;
        ptr->last = NULL;
    } else {
        point->next = ptr;
        ptr->last = point;
        ptr->next = NULL;
        obj->tail = ptr;
    }
}

void print_out(list *obj) {
    node *ptr = obj->head;
    while (ptr != NULL) {
        printf("%c ", ptr->data);
        ptr = ptr->next;
    }
    printf("\n");
}

void free_list(list **obj) {
    node *ptr = (*obj)->head;
    while (ptr != NULL) {
        node *nxt = ptr->next;
        free(ptr);
        ptr = nxt;
    }
    free(*obj);
    *obj = NULL;
}
