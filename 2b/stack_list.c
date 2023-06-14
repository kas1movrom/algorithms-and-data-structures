#include <stdlib.h>
#include <stdio.h>

typedef struct stack{
    char data;
    struct stack *next;
} stack;


int stack_is_empty(stack *st);
int push_stack(stack **st, char data);
char pop_stack(stack **st);
void print_stack(stack *st); // dont use
stack *create_stack(int cnt);
stack *create(); // dont use
void destroy_stack(stack **st);


int stack_is_empty(stack *st) { //
    if (st == NULL) {
        return 1;
    }
    return 0;
}

int push_stack(stack **st, char data) {
    stack *ptr = (stack*)malloc(sizeof(stack));
    ptr->next = *st;
    ptr->data = data;
    *st = ptr;
    return 1;
}

char pop_stack(stack **st) {
    if (!stack_is_empty(*st)) {
        stack *ptr = *st;
        *st = (*st)->next;
        char result = ptr->data;
        free(ptr);
        return result;
    }
    return 0;
}

void print_stack(stack *st) {
    for (stack *ptr = st; ptr != NULL; ptr = ptr->next) {
        printf("%c ", ptr->data);
    }
    printf("\n");
}

stack *create_stack(int cnt) {
    stack *st = NULL;
    cnt++;
    return st;
}

stack *create() {
    stack *st = NULL;
    return st;
}

void destroy_stack(stack **st) {
    if (!stack_is_empty(*st)) {
        stack *ptr = *st;
        while (ptr != NULL) {
            stack *nxt = ptr->next;
            free(ptr);
            ptr = nxt;
        }
        *st = NULL;
    }
}

int create_int(){
    return 1;
}

void print_error(int j) {
	j++;
	printf("Неккоректная запись инфиксной нотации\n");
}
