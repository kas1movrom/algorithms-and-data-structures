#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include "dialog.h"
#include "func.h"
#include "cache.h"
#include "table.h"
#define eof -1
#define end_session 5

typedef struct table {
    char *filename;
    FILE *file;
    int offset;
} table;

typedef int (*t_func) (table *, cache *);



int main()
{

	const char *msgs[] = {"0. Quit", "1. Add", "2. Find one", "3. Find all", 
							"4. Delete one", "5. Delete all", "6.Show table", 
							"7. Show cache"};

	const int cnt_msgs = sizeof(msgs) / sizeof(msgs[0]);
	table *tab = create_table();

	t_func func[8] = {NULL, dialog_add, dialog_find_one, dialog_find_all, 
						dialog_delete_one, dialog_delete_all, dialog_show_table, 
						dialog_show_cache};

	printf("Enter the file name:\n");
	char *filename = readline(">");
	if (filename == NULL)
	{
		free(tab);
		exit(eof);
	}

	if (init_table(filename, tab) == 0) {
		printf("New file opened!\n");
	} else {
		printf("Opened an existing file!\n");
	}

	int size;
	printf("Enter the size of the cache:\n");
	int check = get_positive_int(&size);
	if (check == eof) {
		free(filename);
		fclose(tab->file);
		free(tab);
		exit(eof);
	}

	cache *tab_cache = create_cache(size);

	int rc;
	while ((rc = dialog(msgs, cnt_msgs)) != 0)
	{
		int temp = func[rc](tab, tab_cache);
		if (temp == eof)
			break;
	}

	int cnt = 0;
	key_space **arr = load_in_file(tab, tab_cache, &cnt);
	if (arr != NULL) {
		fclose(tab->file);
		create_final_file(tab, arr, cnt);
	} else {
		fclose(tab->file);
		FILE *file = fopen(filename, "w+b");
		fclose(file);
		destroy_table(tab);
	}
	destroy_cache(tab_cache);
	
	return 0;
}
