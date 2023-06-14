#include <stdio.h>

#include "dialog.h"

typedef int (*t_func) (Graph *);

int main() {
     const char *msgs[] = {"0. Quit", "1. Add vertex", "2. Add edge",
                           "3. Delete vertex", "4. Delete edge",
                           "5. Change vertex type", "6. Print graph", 
                           "7. Exit reachability check", "8. Finding the shortest path",
                           "9. Create dot file", "10. Create spanning graph",
                           "11. Dop process"};

     const int cnt_msgs = sizeof(msgs) / sizeof(msgs[0]);
     Graph *graph = create_graph();

     t_func func[12] = {NULL, dialog_add_vertex, dialog_add_edge,
                       dialog_delete_vertex, dialog_delete_edge,
                       dialog_change_type, dialog_print_graph,
                       dialog_DFS, dialog_Bellman, dialog_dot_file,
                       dialog_spanning_tree, dialog_dop_process};

     int rc;
     while ((rc = dialog(msgs, cnt_msgs)) != 0)
     {
         int temp = func[rc](graph);
         if (temp == -1)
             break;
     }
     destroy_graph(graph);
     return 0;
}
