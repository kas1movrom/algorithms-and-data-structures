#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <math.h>

#define eof -1

#include "table.h"
#include "func.h"
#include "dialog.h"
#include "iterator.h"

typedef struct item {
	unsigned int release;
	unsigned int key;
	char *info;
	struct item *next;
} item;

typedef struct key_space {
	item *head;
} key_space;

typedef struct table {
	key_space **arr;
	int size;
	int cnt;
} table;

typedef int (*t_func) (table *);



int main()
{
	const char *msgs[] = {"0. Quit", "1. Add", "2. Find one",
						  "3. Find all", "4. Delete all", "5. Clean table", 
						  "6. Import from file", "7. Print table"};

	const int cnt_msgs = sizeof(msgs) / sizeof(msgs[0]);

	t_func func[8] = {NULL, dialog_add, dialog_find_one,
					  dialog_find_all, dialog_delete_all,
					  dialog_clean, dialog_import, dialog_print_table};

	int tab_size;
	printf("Enter the table size:\n");
	int check = get_positive_int(&tab_size);
	if (check == eof)
		return eof;

	table *tab = create_table(tab_size);

	int rc;
	while ((rc = dialog(msgs, cnt_msgs)) != 0)
	{
		int tmp = func[rc](tab);
		if (tmp == eof)
			break;
	}		

	destroy_table(tab);
	return 0;			  
}
