#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

typedef struct stack stack;

int stack_is_empty(stack *st);
int push_stack(stack **st, char data);
char pop_stack(stack **st); // dont use
stack *create_stack(int cnt);
stack *create(); // dont use
void destroy_stack(stack **st);

#endif //LAB2_STACK_H
