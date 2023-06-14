#ifndef LABGRAPH_FUNC_H
#define LABGRAPH_FUNC_H

#include "graph.h"

enum FuncResult {
    OKAY,
    DUPLICATE,
    VERTICES_NOT_CLOSE,
    NOT_FOUND_VERTEX,
    NOT_FOUND_PATH
};

enum Bool {
    FALSE,
    TRUE
};

enum Color {
    WHITE,
    GRAY,
    BLACK
};

typedef struct DFS {
    enum Color color;
    int pred;

    int start;
    int finish;
}
DFS;

int find_vertex(Graph *graph, int x, int y);

void init_vertex(Graph *graph, int x, int y, enum VertexType type);

enum Bool check_vertex_on_close(int x1, int y1, int x2, int y2);

void edge_connection(Graph *graph, int index1, int index2);

void swap_vertices(Graph *graph, int index);

void correct_edge(Graph *graph, int j, int i);

void print_vertex(Vertex vertex);

void print_edges(Graph *graph, Vertex vertex);

void DFS_visit(Graph *graph, DFS *array, int index, int *time, enum Bool *result, int *in);

FILE *generate_file(char *filename);

char *get_line(FILE *file);

int make_shield(char *str);

void print_vertex_file(FILE *file, Vertex vertex);

void print_edges_file(FILE *file, Graph *graph, Vertex vertex);

#endif //LABGRAPH_FUNC_H

