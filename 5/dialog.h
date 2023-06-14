#ifndef LABGRAPH_DIALOG_H
#define LABGRAPH_DIALOG_H

#include "func.h"

int get_positive_int(int *value);

int get_number_type(int *type);

int dialog(const char *msgs[], int N);

int dialog_add_vertex(Graph *graph);

int dialog_add_edge(Graph *graph);

int dialog_dop_process(Graph *graph);

int dialog_delete_vertex(Graph *graph);

int dialog_delete_edge(Graph *graph);

int dialog_change_type(Graph *graph);

int dialog_print_graph(Graph *graph);

int dialog_DFS(Graph *graph);

int dialog_Bellman(Graph *graph);

int dialog_dot_file(Graph *graph);

int dialog_spanning_tree(Graph *graph);

#endif //LABGRAPH_DIALOG_H

