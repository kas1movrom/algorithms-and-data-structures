#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include "dialog.h"
#include "func.h"
#include "table.h"

typedef int (*t_func) (table *);

int main()
{
    const char *msgs[] = {"0. Quit", "1. Add", "2. Find one",
                          "3. Find all", "4. Delete one",
                          "5. Delete all","6. Show", "7. Import from file"};
    const int cnt_msgs = sizeof(msgs) / sizeof(msgs[0]);


    table *tab = create_table();
    t_func func[8] = {NULL, dialog_add, dialog_find_one,
                      dialog_find_all, dialog_delete_one, dialog_delete_all,
                      dialog_show_table, dialog_import_file};
    int rc;
    while ((rc = dialog(msgs, cnt_msgs)) != 0)
    {
        int temp = func[rc](tab);
        if (temp == 5)
        {
                break;
        }
    }
    destroy_table(&tab);


    return 0;
}
