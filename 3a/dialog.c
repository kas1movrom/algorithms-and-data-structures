#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include "table.h"
#include "func.h"

typedef struct help {
	char *str;
	unsigned int value;
} help;

typedef struct item {
    unsigned int info;
} item;

typedef struct key_space {
    char *key;
    unsigned int release;
    item *point;
    struct key_space *next;
} key_space;

typedef struct table{
    key_space *elements;
} table;

typedef int (*t_func) (table *);

int dialog(const char *msgs[], int N)
{
        char *errmsg = "";
        int rc;
        int i, n;
        do{
                puts(errmsg);
                errmsg = "Введите корректный номер альтернативы";

                for(i = 0; i < N; ++i)
                        puts(msgs[i]);
                puts("Выберите представленный альтернативы -->");
                while ((n = scanf("%d", &rc)) != 1 || rc < 0 || rc > 7) {
                        if (n == EOF) {
                                rc = 0;
                                break;
                        }
                        printf("\n%s\n", errmsg);
                        n = scanf("%*c");
                }
                if(n == 0)
                        rc = 0;
        } while(rc < 0 || rc >= N);
        return rc;
}



int dialog_add(table *tab)
{
    int info;
    char *key;
    printf("Введите ключ элемента:\n");
    key = readline(">");
    if (key == NULL)
    {
        return 5;
    }
    if (strlen(key) == 0)
    {
        while (strlen(key) == 0) {
                printf("Ключ не может быть пустым!\n");
                free(key);
                key = readline(">");
        }
    }
    int n;
    printf("Введите информацию, содержащуюся в элементе:\n");
    while ((n = scanf("%d", &info)) != 1 || info < 0)
    {
        if (n == EOF)
        {
                free(key);
                return 5;
        }
        printf("Введите беззнаковое целое число!\n");
        scanf("%*c");
    }
    key_space *ptr = add_element(tab, key, info);
    ptr->release++;
    ptr->release--;
    printf("Элемент был успешно добавлен в таблицу\n");
    return 0;
}


int dialog_find_one(table *tab)
{
    char *key;
    if (table_is_empty(tab)) {
        printf("Таблица пуста, поиск невозможен!\n");
        return 0;
    } else {
        printf("Введите ключ элемента, который вы хотите найти:\n");
        key = readline(">");
        if (key == NULL)
        {
                return 5;
        }
        if (strlen(key) == 0)
        {
                while (strlen(key) == 0)
                {
                        printf("Ключ не должен быть пустым\n");
                        free(key);
                        key = readline(">");
                }
        }
        if (!element_is_here(tab, key)) {
            printf("Элемента с данным значением ключа нет!\n");
            free(key);
            return 1;
        } else {
            int cnt = cnt_key(tab, key);
            printf("Содержится %d версия/и/й элемнта с данным ключом", cnt);
            printf("Введите номер версси, которую бы вы хотели найти\n");
            printf("Максимальный номер верси == %d\n", cnt - 1);
            int release;
            int n;
            while ((n = scanf("%d", &release)) != 1 || release < 0 || release > cnt - 1)
            {
                if (n == EOF)
                {
                        free(key);
                        return 5;
                }
                printf("Введите корректный номер версии!\n");
                scanf("%*c");
            }
            table *tab_search = search_this(tab, key, release);
            printf("По вашим запрсом был найден элемент:\n");
            print_table(tab_search);
            free(key);
            destroy_table(&tab_search);
            return 2;
        }
    }

}

int dialog_find_all(table *tab)
{
    char *key;
    if (table_is_empty(tab)) {
        printf("Таблица пуста, поиск невозможен!\n");
        return 0;
    } else {
        printf("Введите ключ элемента, который вы хотите найти:\n");
        key = readline(">");
        if(key == NULL)
        {
                return 5;
        }
        if (strlen(key) == 0)
        {
                while (strlen(key) == 0) {
                        printf("Ключ не должен быть пустым\n");
                        free(key);
                        key = readline(">");
                }
        }
        if (!element_is_here(tab, key)) {
            printf("Элементов с данным значением ключа нет!\n");
            free(key);
            return 1;
        } else {
            table *tab_search = search_all(tab, key);
            printf("По вашим запрсом были найдены элементы:\n");
            print_table(tab_search);
            destroy_table(&tab_search);
            free(key);
            return 2;
        }
    }
}

int dialog_delete_one(table *tab)
{
    char *key;
    if (table_is_empty(tab)) {
        printf("Таблица пуста, удаление невозможно!\n");
        return 0;
    } else {
        printf("Введите ключ элемента, который вы хотите удалить:\n");
        key = readline(">");
        if (key == NULL)
        {
                return 5;
        }
        if (strlen(key) == 0)
        {
                while (strlen(key) == 0)
                {
                        printf("Ключ не должен быть пустым\n");
                        free(key);
                        key = readline(">");

                }
        }
        if (!element_is_here(tab, key)) {
            printf("Элемента с данным значением ключа нет!\n");
            free(key);
            return 1;
        } else {
            int cnt = cnt_key(tab, key);
            printf("Содержится %d версия/и/й элемнта с данным ключом", cnt);
            printf("Введите номер версси, которую бы вы хотели удалить\n");
            printf("Максимальный номер верси == %d\n", cnt - 1);
            int release;
            int n;
            while ((n = scanf("%d", &release)) != 1 || release < 0 || release > cnt - 1)
            {
                if (n == EOF)
                {
                        free(key);
                        return 5;
                }
                printf("Введите корректный номер версии!\n");
                scanf("%*c");
            }
            table *tab_search = delete_this(tab, key, release);
            printf("По вашим запрсом был удален элемент:\n");
            print_table(tab_search);
            free(key);
            destroy_table(&tab_search);
            return 2;
        }
    }
}

int dialog_delete_all(table *tab)
{
    char *key;
    if (table_is_empty(tab)) {
        printf("Таблица пуста, удаление невозможно!\n");
        return 0;
    } else {
        printf("Введите ключ элемента, который вы хотите удалить:\n");
        key = readline(">");
        if (key == NULL)
        {
                return 5;
        }
        if (strlen(key) == 0)
        {
                while (strlen(key) == 0)
                {
                        printf("Ключ не должен быть пустым\n");
                        free(key);
                        key = readline(">");
                }
        }
        if (!element_is_here(tab, key)) {
            printf("Элементов с данным значением ключа нет!\n");
            free(key);
            return 1;
        } else {
            table *tab_search = delete_all(tab, key);
            printf("По вашим запрсом были удалены элементы:\n");
            print_table(tab_search);
            destroy_table(&tab_search);
            free(key);
            return 2;
        }
    }
}

int dialog_show_table(table *tab)
{
    printf("Ваша таблица:\n");
    print_table(tab);
    return 0;
}

int dialog_import_file(table *tab)
{
	if (!table_is_empty(tab)) {
		printf("Таблица не пуста, импорт из файла невозможен!\n");
		return 0;
	} 
	printf("Введите имя файла:\n");
	char *filename = readline(">");
	if (filename == NULL)
	{
		return 5;
	}

	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		printf("Ошибка при открытии файла!\n");
		return 1;
	} 

	char *line = scan(file);
	while (line != NULL)
	{
		help *element = make_struct(line);
		if (element == NULL)
		{
			printf("Файл записан не в нужном формате!");
			free(filename);
			fclose(file);
			return 2;
		}
		key_space *ptr = add_element(tab, element->str, element->value);
		element->str = NULL;
		free(element);
		ptr->release--;
		ptr->release++;
		line = scan(file);
	}
	
	printf("Запись прошла успешно!\n");
	free(filename);
	fclose(file);
}
