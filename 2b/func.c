#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define input "+-*()qwertyuiopasdfghjklzxcvbn/m"
#include "stack.h"
#include "storage.h"

int check1(char *str, int length)
{
    for (int i = 0; i < length; i++) {
        if (strchr(input, (int)str[i]) == NULL)
            return 0;
    }
    return 1;
}

int check2(char *str, int length, int *op, int *con,
           int *op_bracket, int *cl_bracket) {
    for (int i = 0; i < length; i++) {
        if (str[i] >= '*' && str[i] <= '/')
            *op = *op + 1;

        if (str[i] >= 'a' && str[i] <= 'z')
            *con = *con + 1;

        if (str[i] == '(')
            *op_bracket = *op_bracket + 1;

        if (str[i] == ')')
            *cl_bracket = *cl_bracket + 1;

        if (*cl_bracket > *op_bracket)
            return 0;
    }
    return 1;
}

int check3(char *str, int length)
{
    for (int i = 0; i < length - 1; i++) {
        if ((str[i] >= '*' && str[i] < '/' && str[i + 1] == ')') ||
            (str[i] == '(' && str[i + 1] >= '*' && str[i + 1] <= '/') ||
            (str[i] >= 'a' && str[i] <= 'z' && str[i + 1] == '(') ||
            (str[i] == ')' && str[i + 1] >= 'a' && str[i + 1] < 'z') ||
            (str[i] >= 'a' && str[i] <= 'z' && str[i + 1] >= 'a' && str[i + 1] <= 'z') ||
            (str[i] >= '*' && str[i] <= '/' && str[i + 1] >= '*' && str[i + 1] <= '/') ||
            (str[i] == '(' && str[i + 1] == ')'))
            return 0;
    }
    return 1;
}

int check_formula(char *str) {
    if (str != NULL) {
        int length = (int)strlen(str);
        if (length > 2) {

            if (!check1(str, length))
                return 0;

            int op = 0;
            int con = 0;
            int op_bracket = 0;
            int cl_bracket = 0;

            if (!check2(str, length, &op, &con, &op_bracket, &cl_bracket))
                return 0;

            if (op == con - 1 && op_bracket == cl_bracket) {
                if (!check3(str, length))
                    return 0;

                return 1;
            }
            return 0;
        }
        return 0;
    }
    return 0;
}

char *get_str() { // returned input string
    char buf[81];
    int len = 0;
    int n;
    char *arr = (char*)malloc(sizeof(char));
    *arr = '\0';
    do {
        n = scanf("%80[^\n]", buf);
        if (n == 0) {
            scanf("%*c");
        }
        if (n < 0) {
            free(arr);
            arr = NULL;
        }
        if (n > 0) {
            len += (int)strlen(buf);
            arr = (char*)realloc(arr, sizeof(char) * len + 1);
            strcat(arr, buf);
        }
    } while (n > 0);
    return arr;
}

int individual_task_this(stack **stack_op, list *obj, char *str) {
    for (int i = 0; i < (int)strlen(str); i++) {

        if (str[i] >= 'a' && str[i] <= 'z') {
            push_out_back(obj, str[i]);
        }

        if (str[i] == '(') {
            if(!push_stack(stack_op, str[i])) {
                goto returned;
            }
        }

        if (str[i] == ')') {

            char value = pop_stack(stack_op);
            while (value != '(') {
                push_out_back(obj, value);
                value = pop_stack(stack_op);
            }

        }

        if (str[i] == '*' || str[i] == '/') {
            char flag = 1;
            while (flag)
            {
                if (stack_is_empty(*stack_op)) {
                    if (!push_stack(stack_op, str[i])) {
                        goto returned;
                    }
                    flag = 0;
                } else {
                    char value = pop_stack(stack_op);

                    if (value == '*' || value == '/')
                        push_out_back(obj, value);

                    if (value == '+' || value == '-' || value == '(') {
                        if (!push_stack(stack_op, value)) {
                            goto returned;
                        }
                        if (!push_stack(stack_op, str[i])) {
                            goto returned;
                        }
                        flag = 0;
                    }
                }

            }
        }

        if (str[i] == '+' || str[i] == '-') {
            char flag = 1;
            while (flag)
            {
                if (stack_is_empty(*stack_op)) {
                    if (!push_stack(stack_op, str[i])) {
                        goto returned;
                    }
                    flag = 0;
                } else {
                    char value = pop_stack(stack_op);

                    if (value == '*' || value == '/' || value == '+' || value == '-')
                        push_out_back(obj, value);

                    if (value == '(') {
                        if (!push_stack(stack_op, value)) {
                            goto returned;
                        }
                        if (!push_stack(stack_op, str[i])) {
                            goto returned;
                        }
                        flag = 0;
                    }
                }
            }

        }
    }
    while (!stack_is_empty(*stack_op))
        push_out_back(obj, pop_stack(stack_op));

    return 1;
    returned: return 0;
}
