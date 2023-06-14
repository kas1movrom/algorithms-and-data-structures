#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "storage.h"
#include "func.h"

int main()
{
    char *str;
    int cnt = create_int();

    printf("Правила ввода исходного выражения:\n");
    printf("\n\nЗапрещено:\n");
    printf("-Вводить любые символы кроме нижнего регистра латинского алфавита, +, -, *, (, ), /. \n");
    printf("-Вводить неккоректные выражения с точки зрения инфиксной нотации.\n");
    printf("-Вводить сочетания: +), (+, а(, )с, ас, ++, ()\n");
    printf("-Количество операндов должно быть на один больше, чем количество операторов\n");
    printf("-Количество открывающих скобок должно равняться количеству закрывающих скобок\n");
    printf("\n\n");
    printf("Вместо (-а+с) требуется вводить (с-а)\n\n");




    printf("Инфикс: ");
    int j = 0;
    while ((str = get_str())) {

        if (check_formula(str)) {

            //int cnt = create_int();
            stack *stack_op = create_stack(cnt);
            //stack *stack_op = create();

            list *obj = create_out_list();

            if (!individual_task_this(&stack_op, obj, str)) {
                printf("Выделенной памяти недостаточно!\n");
                goto destroyed;
            }


            printf("Постфикс: ");
            print_out(obj);

            destroyed: destroy_stack(&stack_op);

            free_list(&obj);
            printf("Инфикс: ");

        } else {

            print_error(j);
          	/*if (j != 0) {
          		printf("Неккоректная запись инфиксной нотации\n");
          	}*/

        }
        free(str);
        j++;
    }
    return 0;
}
