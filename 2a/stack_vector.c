#include <stdio.h>
#include <stdlib.h>

typedef struct stack {
    unsigned int top;
    unsigned int cnt;
    char *arr;
} stack;

int stack_is_full(stack *st);
void init_stack(stack *st, int cnt);


int stack_is_empty(stack *st);
int push_stack(stack **st, char data);
char pop_stack(stack **st);
void print_stack(stack *st);
stack *create_stack(int cnt);
stack *create();
void destroy_stack(stack **st);


int push_stack(stack **st, char data) {
    if (!stack_is_full(*st)) {
        (*st)->arr[(*st)->top] = data;
        (*st)->top = (*st)->top + 1;
        return 1;
    } else {
        return 0;
    }
}
char pop_stack(stack **st) {
    if (!stack_is_empty(*st)) {
        char res = (*st)->arr[(*st)->top - 1];
        (*st)->top = (*st)->top - 1;
        return res;
    }
    return 0;
}

int stack_is_full(stack *st) {
    if (st->top == st->cnt) {
        return 1;
    }
    return 0;
}

int stack_is_empty(stack *st) {
    if (st->top == 0) {
        return 1;
    }
    return 0;
}

void print_stack(stack *st) {
    if (!stack_is_empty(st)) {
        for (int i = st->top - 1; i >= 0; i--)
            printf("%c ", st->arr[i]);
    }
    printf("\n");
}

void init_stack(stack *st, int cnt) {
    st->top = 0;
    st->cnt = cnt;
    st->arr = (char*)malloc(sizeof(char) * st->cnt);
}

stack *create() {
    int cnt;
    printf("Введите количество желаемой выделеяемой памяти\n");
    scanf("%d", &cnt);
    stack *st = (stack*)malloc(sizeof(stack));
    init_stack(st, cnt);
    return st;
}


stack *create_stack(int cnt) {
    stack *st = (stack*)malloc(sizeof(stack));
    init_stack(st, cnt);
    return st;
}

void destroy_stack(stack **st) {
    free((*st)->arr);
    free((*st));
}

int create_int() {
    printf("Введите размер памяти под стек\n");
    int cnt;
    while (scanf("%d", &cnt) != 1 || cnt <= 0) {
        scanf("%*c");
        printf("Введите целое число, большое нуля!\n");
    }
    return cnt;
}

void print_error(int j) {
	if (j != 0) {
		printf("Неккоректная запись инфиксной нотации\n");
	}
}
