#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"
#define sep "|"

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

key_space *add_element(table *tab, char *key, unsigned int info)
{
    key_space *element = create_element(key, info);
    if (table_is_empty(tab)) {
        tab->elements = element;
        element->next = NULL;
        element->release = 0;

    } else {
        int cnt = 0;
        for (key_space *iter = element_first(tab); !element_done(iter); 
        				iter = element_next(iter)){
        	if (!strcmp(element->key, iter->key))
        		cnt++;				
        }
        key_space *ptr = tab->elements;
        tab->elements = element;
        element->next = ptr;
        element->release = cnt;

    }
    return element;
}

int destroy_table(table **tab)
{
    if (table_is_empty(*tab)) {
        free(*tab);
        *tab = NULL;
        return 0;
    }

    key_space *ptr = (*tab)->elements;
    while (ptr != NULL)
    {
        key_space *nxt = ptr->next;
        free(ptr->point);
        free(ptr->key);
        free(ptr);
        ptr = nxt;
    }
    free(*tab);
    *tab = NULL;

    return 1;
}

table *delete_all(table *tab, char *key)
{
    table *res_tab = create_table();

    if (!table_is_empty(tab))
    {
        key_space *ptr = tab->elements;
        while (ptr->next != NULL)
        {
            if (strcmp(ptr->next->key, key) == 0) {
                key_space *el = ptr->next;
                ptr->next = el->next;
                unsigned int val = add_element_exising(res_tab, el);
                val--;
            } else {
                ptr = ptr->next;
            }
        }
        ptr = tab->elements;
        if (strcmp(ptr->key, key) == 0)
        {
            key_space *this = ptr;
            tab->elements = this->next;
            unsigned int val = add_element_exising(res_tab, this);
            val--;
        }
    }

    return res_tab;
}

table *delete_this(table *tab, char *key, unsigned int release)
{
    table *res_tab = create_table();
    if (!table_is_empty(tab))
    {
        key_space *ptr = tab->elements;
        while (ptr->next != NULL)
        {
            if (strcmp(ptr->next->key, key) == 0 && ptr->next->release == release) {
                dias(tab, ptr->next);
                key_space *el = ptr->next;
                ptr->next = el->next;
                unsigned int val = add_element_exising(res_tab, el);
                val--;
                goto DO_RETURN1;
            }
            ptr = ptr->next;
        }
        ptr = tab->elements;
        if (strcmp(ptr->key, key) == 0)
        {
            key_space *this = ptr;
            tab->elements = this->next;
            unsigned int val = add_element_exising(res_tab, this);
            val--;
        }
    }
    DO_RETURN1 : return res_tab;
}

table *search_all(table *tab, char *key)
{
    table *res_tab = create_table();

    if (!table_is_empty(tab))
    {
        for (key_space *ptr = element_first(tab); !element_done(ptr);
        				ptr = element_next(ptr)) {
        	if (!strcmp(ptr->key, key))
        		add_element_exising(res_tab, create_copy(ptr));					

        }
    }

    return res_tab;
}

table *search_this(table *tab, char *key, unsigned int release)
{
    table *res_tab = create_table();

    if (!table_is_empty(tab))
    {
        for (key_space *ptr = element_first(tab); !element_done(ptr);
        				ptr = element_next(ptr)) {
        	if (strcmp(ptr->key, key) == 0 && ptr->release == release)
        	{
        		add_element_exising(res_tab, create_copy(ptr));
        		goto DO_RETURN;
        	}					
       	}

    }

    DO_RETURN : return res_tab;
}

help *make_struct(char *line)
{
	int cnt = 0;
	char *portion = strtok(line, sep);
	help *obj = (help*)malloc(sizeof(help));
	int errors_cnt = 0;
	while (portion != NULL)
	{
		if (cnt == 0)
		{
			obj->str = portion;
		}
		if (cnt == 1)
		{
			for (int i = 0; i < (int)strlen(portion); i++) {
				if (portion[i] < '0' || portion[i] > '9')
				{
					errors_cnt++;
				}
			}
			if (errors_cnt > 0) {
				obj->value = -1;
			} else {
				obj->value = atoi(portion);
			}
		}
		cnt++;
		portion = strtok(NULL, sep);
	}
	if (cnt != 2 || errors_cnt > 0)
	{
		free(obj->str);
		free(obj);
		obj = NULL;
	}
	return obj;
}

char *scan(FILE* filename){
	char *ptr = (char *)malloc(1);
	char buf[81];
	int n, len = 0;
	*ptr = '\0';
	do{
		n = fscanf(filename,"%80[^\n]", buf);
	       	if(n < 0){
			free(ptr);
			ptr = NULL;
			continue;
		}
		if(n == 0){
			fscanf(filename,"%*c");
		}
		else {
			len += strlen(buf);
			ptr = (char *) realloc(ptr, len + 1);
			strcat(ptr, buf);
		}
	}
  	while(n > 0);
  	return ptr;

}


void print_table(table *tab)
{
    if (table_is_empty(tab)) {

        printf("она пуста\n");

    } else {

        key_space *ptr = tab->elements;
        while (ptr != NULL)
        {
            printf("%s. %d -- %d\n", ptr->key, ptr->point->info, ptr->release);
            ptr = ptr->next;
        }
        printf("\n");
    }
}
