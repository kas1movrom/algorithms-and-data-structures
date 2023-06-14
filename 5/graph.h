#ifndef LABGRAPH_GRAPH_H
#define LABGRAPH_GRAPH_H

#include <stdio.h>

typedef struct Bellman {
    int pred;
    int distance;
}
Bellman;


enum VertexType {
    ENTER,
    EXIT,
    CONNECT
};

typedef struct Edge {
    int from;
    int to;
}
Edge;

typedef struct Vertex {

    int x;
    int y;
    enum VertexType type;

	int count;
    Edge *edges;
}
Vertex;

typedef struct Graph {
    int capacity;
    int size;
    Vertex *vertices;
}
Graph;

Graph *create_graph();

enum FuncResult add_vertex(Graph *graph, int x, int y, enum VertexType type);

enum FuncResult add_edge(Graph *graph, int x1, int y1, int x2, int y2);

enum FuncResult delete_vertex(Graph *graph, int x, int y);

enum FuncResult delete_edge(Graph *graph, int x1, int y1, int x2, int y2);

enum FuncResult change_type(Graph *graph, int x, int y, enum VertexType type);

void print_graph(Graph *graph);

void destroy_graph(Graph *graph);

enum FuncResult depth_first_search(Graph *graph, int x, int y, int *in);

Bellman *BellmanFord(Graph *graph, int x1, int y1, int x2, int y2, int *index);

enum FuncResult create_dot_file(FILE *file, Graph *graph);

enum FuncResult minimum_spanning_tree(Graph *graph);

enum FuncResult dop_process(FILE *file, Graph *graph);

void print_graph_file(FILE *file, Graph *graph);

#endif //LABGRAPH_GRAPH_H
