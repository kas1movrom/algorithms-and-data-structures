#include <stdio.h>
#include <stdlib.h>

#include "func.h"

#include <string.h>

#define MAX_EDGES 4

#define MAX_ELEMENTS 5000

int find_vertex(Graph *graph, int x, int y)
{
    for (int i = 0; i < graph->size; ++i) {
        if (graph->vertices[i].x == x && graph->vertices[i].y == y)
            return i;
    }
    return -1;
}

char *get_line(FILE *file)
{
	char *ptr = (char*)malloc(sizeof(char));
	char buf[81];
	int n, len = 0;
	*ptr = '\0';
	do {
		n = fscanf(file, "%80[^\n]", buf);
		if (n < 0)
		{
			free(ptr);
			ptr = NULL;
			continue;
		}
		if (n == 0)
		{
			fscanf(file, "%*c");
		}
		if (n > 0)
		{
			len += (int)strlen(buf);
			ptr = (char*)realloc(ptr, len + 1);
			strcat(ptr, buf);
		}
	} while(n > 0);
	return ptr;
}

int make_shield(char *str)
{
	return atoi(str);
}

void init_vertex(Graph *graph, int x, int y, enum VertexType type)
{
    graph->vertices[graph->size].count = 0;
    graph->vertices[graph->size].x = x;
    graph->vertices[graph->size].y = y;

    graph->vertices[graph->size].type = type;
    graph->vertices[graph->size].edges = (Edge*) malloc(sizeof(Edge) * MAX_EDGES);

    for (int i = 0; i < MAX_EDGES; ++i) {
        graph->vertices[graph->size].edges[i].to = -1;
        graph->vertices[graph->size].edges[i].from = -1;
    }
}

enum Bool check_vertex_on_close(int x1, int y1, int x2, int y2)
{
    return ((x1 == x2) && abs(y1 - y2) == 1) || ((y1 == y2) && abs(x1 - x2) == 1);
}

void edge_connection(Graph *graph, int index1, int index2)
{
    int count = ++graph->vertices[index1].count;

    graph->vertices[index1].edges[count - 1].from = index1;
    graph->vertices[index1].edges[count - 1].to = index2;
}

void swap_vertices(Graph *graph, int index)
{
    graph->vertices[index].edges = graph->vertices[index + 1].edges;
    graph->vertices[index].x = graph->vertices[index + 1].x;
    graph->vertices[index].y = graph->vertices[index + 1].y;

    graph->vertices[index].type = graph->vertices[index + 1].type;
    graph->vertices[index].count = graph->vertices[index + 1].count;
}

void correct_edge(Graph *graph, int j, int i)
{
    for (int k = j; k < graph->vertices[i].count - 1; ++k) {
        graph->vertices[i].edges[k].to = graph->vertices[i].edges[k + 1].to;
        graph->vertices[i].edges[k].from = graph->vertices[i].edges[k + 1].from;
    }
    --graph->vertices[i].count;
}

void print_vertex(Vertex vertex)
{
    printf("{%d, %d, ", vertex.x, vertex.y);
    if (vertex.type == ENTER)
        printf("ENTER}");

    if (vertex.type == EXIT)
        printf("EXIT}");

    if (vertex.type == CONNECT)
        printf("CONNECT}");
}

void print_vertex_file(FILE *file, Vertex vertex)
{
    fprintf(file, "{%d, %d, ", vertex.x, vertex.y);
    if (vertex.type == ENTER)
        fprintf(file, "ENTER}");

    if (vertex.type == EXIT)
        fprintf(file, "EXIT}");

    if (vertex.type == CONNECT)
        fprintf(file, "CONNECT}");	
}

void print_edges_file(FILE *file, Graph *graph, Vertex vertex)
{
	print_vertex_file(file, vertex);
	fprintf(file, " --> ");
	for (int i = 0; i < vertex.count; ++i) {
		print_vertex_file(file, graph->vertices[vertex.edges[i].to]);
		fprintf(file, " ");
	}
	fprintf(file, "\n");
}

void print_edges(Graph *graph, Vertex vertex)
{
    print_vertex(vertex);
    printf(" --> ");
    for (int i = 0; i < vertex.count; ++i) {
        print_vertex(graph->vertices[vertex.edges[i].to]);
        printf(" ");
    }
    printf("\n");
}

void DFS_visit(Graph *graph, DFS *array, int index, int *time, enum Bool *result, int *in)
{
    array[index].color = GRAY;
    *time += 1;
    array[index].start = *time;
    int j;
    for (int i = 0; i < graph->vertices[index].count; ++i) {
        j = graph->vertices[index].edges[i].to;
        if (array[j].color == WHITE) {
            if (graph->vertices[j].type == EXIT) {
            	*in = j;
                *result = TRUE;
			}
            array[j].pred = index;
            DFS_visit(graph, array, j, time, result, in);
        }
    }
    array[index].color = BLACK;
    *time += 1;
    array[index].finish = *time;
}

FILE *generate_file(char *filename)
{
	FILE *file = fopen(filename, "w");
	for (int i = 0; i < 6000; ++i)
	{
		fprintf(file, "%d %d %d\n", rand() % 100 + 1, rand() % 100 + 1, rand() % 3);
	}
	return file;
}
