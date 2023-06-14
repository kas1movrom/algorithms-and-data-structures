#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include "dialog.h"
#include "func.h"
#include "tree.h"

#define SUCCESS 1
#define FAIL 0

#define eof -1

typedef struct knot {
    unsigned int key;
    char *info;

    struct knot *right;
    struct knot *left;
    struct knot *parent;
    struct knot *detour;
} knot;

typedef struct tree {
    knot *root;
} tree;

typedef int (*t_func) (tree *);

int main()
{
    const char *msgs[] = {"0. Quit", "1. Add", "2. Delete key",
                              "3. Detour tree", "4. Find key", "5. Special find",
                              "6. Import from file", "7. Print tree", "8. Print tree list", 
                              "9. Create a .dot file"};

    const int cnt_msgs = sizeof(msgs) / sizeof(msgs[0]);

    t_func func[10] = {NULL, dialog_add, dialog_delete_element, dialog_detour_tree, 
        				dialog_find_element, dialog_special_search, dialog_import_from_file, 
        				dialog_print_tree, dialog_print_tree_list, dialog_dot_file};	   	

	tree *wood = create_tree();

	int rc;
	while ((rc = dialog(msgs, cnt_msgs)) != 0)
	{
		int tmp = func[rc](wood);
		if (tmp == eof)
			break;
	}

	destroy_tree(wood);
   	

    return 0;
}
