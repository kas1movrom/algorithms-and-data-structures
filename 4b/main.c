 #include <stdio.h>
 #include <stdlib.h>
 #include <readline/readline.h>

 #include "dialog.h"

 int main()
 {
    const char *msgs[] = {"0. Quit", "1. Add", "2. Delete key",
                              "3. Detour tree", "4. Find key", "5. Special find",
                              "6. Import from file", "7. Print tree", "8. Print graph", 
                              "9. Process file"};

    const int cnt_msgs = sizeof(msgs) / sizeof(msgs[0]);

    t_func func[10] = {NULL, dialog_add, dialog_delete_element, dialog_detour_tree,
                                        dialog_find_element, dialog_special_search, dialog_import_from,
                                        dialog_print_tree, dialog_dot_file, dialog_process_file};

    tree *wood = create_tree();

    int rc;
    while ((rc = dialog(msgs, cnt_msgs)) != 0)
    {
        int tmp = func[rc](wood);
        if (tmp == -1)
            break;
    }

    delete_tree(wood->root);
    free(wood);

	
	return 0;
 }
