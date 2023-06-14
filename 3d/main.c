#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#include "dialog.h"
#include "table.h"
#include "func.h"

#define eof -1
#define BYTES_SIZE 4
#define ONE_BIT 256
#define HASH_NUMBER 37

typedef struct key_space {
    int offset;
    int len;
    int key;
    int release;
    char *info;
} key_space;

typedef struct table {
    char *filename;
    FILE *file;
    int offset;
    int size;
} table;

typedef int (*t_func) (table *);



int main()
{
	const char *msgs[] = {"0. Quit", "1. Add", "2. Find one",
						  "3. Find all", "4. Delete all",
						   "5. Clean table", "6. Show table"};

	const int cnt_msgs = sizeof(msgs) / sizeof(msgs[0]);

	table *tab = create_table();

	t_func func[7] = {NULL, dialog_add, dialog_find_one,
					  dialog_find_all, dialog_delete_all,
					  dialog_clean, dialog_print_table};

	printf("Enter the file name:\n");
	char *filename = readline(">");
	if (filename == NULL)
	{
		free(tab);
		exit(eof);
	}

	init_table(filename, tab);

	int rc;
	while ((rc = dialog(msgs, cnt_msgs)) != 0)
	{
		int tmp = func[rc](tab);
		if (tmp == eof)
			break;
	}
	int size = 0;
	key_space **arr = load_in_file(tab, &size);
	if (arr != NULL) {
		fclose(tab->file);
		create_final_file(tab, arr, size);
		return 0;
	} else {
		fclose(tab->file);
		FILE *file = fopen(tab->filename, "w+b");
		fwrite(&(tab->size), sizeof(int), 1, file);
		fclose(file);
		destroy_table(tab);
	}

	return 0;
	
}
