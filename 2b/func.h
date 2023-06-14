#ifndef FUNC_H_INCLUDED
#define FUNC_H_INCLUDED

typedef struct stack stack;
typedef struct list list;

char *get_str();

int check1(char *str, int length);
int check2(char *str, int length, int *op, int *con, int *op_bracket, int *cl_bracket);
int check3(char *str, int length);
int check_formula(char *str);
int create_int();
int individual_task_this(stack **stack_op, list *obj, char *str);
void print_error(int j);

#endif
