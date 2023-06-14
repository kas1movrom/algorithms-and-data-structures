#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "cache_dialog.h"

typedef int (*c_func) (tree *, cache *);

int main()
{
	const char *msgs[] = {"0. Quit", "1. Add", "2. Delete", "3. Detour", "4. Find", "5. Print"};

	const int cnt_msgs = sizeof(msgs) / sizeof(msgs[0]);

	c_func func[6] = {NULL, dialog_cache_add, dialog_cache_delete_element, dialog_cache_detour_tree, 
						dialog_cache_find_element, dialog_cache_print_tree};

	printf("Enter size of cache:\n");
	int size;
	scanf("%d", &size);
	cache *tab = create_cache(size);

	tree *wood = create_tree();

	int rc;
	while ((rc = dialog_cache(msgs, cnt_msgs)) != 0)
	{
		int tmp = func[rc](wood, tab);
		if (tmp == -1)
			break;
	}

	free_cache(tab, wood);

	delete_tree(wood->root);
	free(wood);

	
}
