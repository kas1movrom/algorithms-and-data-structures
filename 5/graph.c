#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "func.h"

#define START_CAPACITY 50

#include <string.h>
#include "dialog.h"

const char *graph_msg[] = {
	"Successfully\n",
	"Duplicate point\n",
	"Vertices not close\n",
	"Cant find vertex\n",
	"Cant found path\n"
};

#define sep " "

Graph *create_graph()
{
    Graph *graph = (Graph *) malloc(sizeof(Graph));
    graph->size = 0;
    graph->capacity = START_CAPACITY;

    graph->vertices = (Vertex *) malloc(sizeof(Vertex) * START_CAPACITY);
    return graph;
}

enum FuncResult add_vertex(Graph *graph, int x, int y, enum VertexType type)
{
    if (find_vertex(graph, x, y) != -1)
        return DUPLICATE;

    if (graph->size == graph->capacity)
    {
        graph->capacity *= 2;
        graph->vertices = (Vertex *) realloc(graph->vertices, sizeof(Vertex) * graph->capacity);
    }
    init_vertex(graph, x, y, type);

    ++graph->size;
    return OKAY;
}

enum FuncResult dop_process(FILE *file, Graph *graph)
{
	char *line = get_line(file);
	while (line != NULL)
	{
		int array[3], cnt = 0;
		char *token = strtok(line, sep);
		while (token != NULL)
		{
			array[cnt] = make_shield(token);
			++cnt;
			token = strtok(NULL, sep);
		}
		
		add_vertex(graph, array[0], array[1], array[2]);

		int random = rand() % 4;

		if (random != 0)
			add_edge(graph, array[0], array[1], array[0], array[1] - 1);
		if (random != 1)	
			add_edge(graph, array[0], array[1], array[0], array[1] + 1);
		if (random != 2)	
			add_edge(graph, array[0], array[1], array[0] - 1, array[1]);
		if (random != 3)
			add_edge(graph, array[0], array[1], array[0] + 1, array[1]);
		
		free(line);
		line = get_line(file);
	}

	char *filename1 = "reachability_check.txt";
	//char *filename2 = "path_from_enter.txt";
	char *filename3 = "graph_after_spanning.txt";

	FILE *file1 = fopen(filename1, "w");
	//FILE *file2 = fopen(filename2, "w");
	FILE *file3 = fopen(filename3, "w");

	int in = -1;
	for (int i = 0; i < graph->size; ++i) {
		if (graph->vertices[i].type == ENTER) {
			print_vertex_file(file1, graph->vertices[i]);
			fprintf(file1, ":");
			int x = graph->vertices[i].x, y = graph->vertices[i].y;
			fprintf(file1, "%s", graph_msg[depth_first_search(graph, x, y, &in)]);
			if (in != -1) {
				print_vertex_file(file1, graph->vertices[in]);
				fprintf(file1, "\n\n");
			} else {
				fprintf(file1, "\n");
			}
			/*if (in != -1)
			{
				int idex = 0;
				Bellman *array = BellmanFord(graph, x, y, graph->vertices[in].x, graph->vertices[in].y, &idex);
				if (array != NULL)
				{
					int pred = idex, length = 0;
					while (1)
					{
						print_vertex_file(file1, graph->vertices[pred]);
						++length;
						pred = array[pred].pred;
						if (pred == -1)
							break;

						fprintf(file1, " <-- ");
					}
					fprintf(file1, "\nlength : %d\n", length - 1);
					free(array);
				}
			}*/
		}
	}

	/*for (int i = 0; i < graph->size; ++i) {
		if (graph->vertices[i].type == ENTER && depth_first_search(graph, graph->vertices[i].x, graph->vertices[i].y, &in) == OKAY)
		{
			int x = graph->vertices[i].x, y = graph->vertices[i].y;
			for (int j = 0; j < graph->size; ++j) {
				if (graph->vertices[j].type == EXIT)
				{
					int index = 0;
					Bellman *array = BellmanFord(graph, x, y, graph->vertices[j].x, graph->vertices[j].x, &index);
					//printf("in bellman\n");
					if (array != NULL) {
						int pred = index, length = 0;
						while (1)
						{
							print_vertex_file(file2, graph->vertices[pred]);
							++length;
							pred = array[pred].pred;
							if (pred == -1)
								break;	

							fprintf(file2, " <-- ");
						}
						fprintf(file2, "\nlength : %d\n", length - 1);
						free(array);
						goto begin;
					}
					//goto begin;
				}
			}
			break;
		}
	}*/

	fclose(file1);
	//fclose(file2);

	fprintf(file3, "GRAPH BEFORE SPANNING:\n");
	print_graph_file(file3, graph);
	
	// printf("GRAPH BEFORE SPANNING:\n");
	// print_graph(graph);
	
	minimum_spanning_tree(graph);
	

	fprintf(file3, "\nGRAPH AFTER SPANNING:\n");
	print_graph_file(file3, graph);
	fclose(file3);
	// print_graph(graph);
	//dialog_dot_file(graph);
	return OKAY;
}

enum FuncResult minimum_spanning_tree(Graph *graph)
{
    int index = -1;
    int ing = -1;
    for (int i = 0; i < graph->size; ++i) {
        if (graph->vertices[i].type == ENTER &&
                depth_first_search(graph, graph->vertices[i].x, graph->vertices[i].y, &ing) == OKAY) {
            print_vertex(graph->vertices[i]);
            index = i;
            break;
        }
    }
    if (index == -1)
        return NOT_FOUND_VERTEX;

    DFS *array = (DFS *) malloc(sizeof(DFS) * graph->size);
    for (int i = 0; i < graph->size; ++i) {
        array[i].color = WHITE;
        array[i].pred = -1;
    }

    int time = 0;
    int in = -1;
    enum Bool result = FALSE;
    DFS_visit(graph, array, index, &time, &result, &in);

	/*printf("test:\n");
    for (int i = 0; i < graph->size; ++i) {
    	print_vertex(graph->vertices[i]);
    	printf("%d\n", array[i].pred);
    }*/

    for (int i = 0; i < graph->size; ++i) {
        if (array[i].pred != -1) {
            int x = graph->vertices[i].x;
            int y = graph->vertices[i].y;
            //printf("%d, %d\n", x, y);

            //printf("%d, %d\n", graph->vertices[array[i].pred].x, graph->vertices[array[i].pred].y);

            if (graph->vertices[array[i].pred].x == x - 1 && graph->vertices[array[i].pred].y == y) {
            	//printf("block1\n");
                delete_edge(graph, x + 1, y, x, y);
                delete_edge(graph, x, y + 1, x, y);
                delete_edge(graph, x, y - 1, x, y);
            } else if (graph->vertices[array[i].pred].x == x + 1 && graph->vertices[array[i].pred].y == y) {
            	//printf("block2\n");
                delete_edge(graph, x - 1, y, x, y);
                delete_edge(graph, x, y + 1, x, y);
                delete_edge(graph, x, y - 1, x, y);
            } else if (graph->vertices[array[i].pred].x == x && graph->vertices[array[i].pred].y == y - 1) {
				//printf("block3\n");
                delete_edge(graph, x, y + 1, x, y);
                delete_edge(graph, x - 1, y, x, y);
                delete_edge(graph, x + 1, y, x, y);
            } else if (graph->vertices[array[i].pred].x == x && graph->vertices[array[i].pred].y == y + 1) {
				//printf("block4\n");
                delete_edge(graph, x, y - 1, x, y);
                delete_edge(graph, x - 1, y, x, y);
                delete_edge(graph, x + 1, y, x, y);
            }
        }
    }

    int *array_x = (int*)malloc(sizeof(int) * 10);
    int *array_y = (int*)malloc(sizeof(int) * 10);
    int size = 0;
    for (int i = 0; i < graph->size; ++i) {
        if (array[i].pred == -1 && i != index)
        {
            ++size;
            array_x = (int*) realloc(array_x, sizeof(int) * size);
            array_y = (int*) realloc(array_y, sizeof(int) * size);
            array_x[size - 1] = graph->vertices[i].x;
            array_y[size - 1] = graph->vertices[i].y;
        }
    }

    for (int i = 0; i < size; ++i) {
        delete_vertex(graph, array_x[i], array_y[i]);
    }
    free(array_x);
    free(array_y);
   

    free(array);
    return OKAY;
}

enum FuncResult create_dot_file(FILE *file, Graph *graph)
{
    for (int i = 0; i < graph->size; ++i) {
        for (int j = 0; j < graph->vertices[i].count; ++j) {
            fprintf(file, "\t%c%d;%d%c -> %c%d;%d%c", 34, graph->vertices[i].x,
                    graph->vertices[i].y, 34, 34, graph->vertices[graph->vertices[i].edges[j].to].x,
                    graph->vertices[graph->vertices[i].edges[j].to].y, 34);

            if (graph->vertices[i].type == ENTER) {
                fprintf(file, " [color = blue];\n");
            } else {
                if (graph->vertices[graph->vertices[i].edges[j].to].type == EXIT) {
                    fprintf(file, " [color = red];\n");
                } else {
                    fprintf(file, ";\n");
                }
            }
        }
    }
    return OKAY;
}

enum FuncResult add_edge(Graph *graph, int x1, int y1, int x2, int y2)
{
    if (check_vertex_on_close(x1, y1, x2, y2) == FALSE)
        return VERTICES_NOT_CLOSE;

    int index1, index2;
    if ((index1 = find_vertex(graph, x1, y1)) == -1 || (index2 = find_vertex(graph, x2, y2)) == -1)
        return NOT_FOUND_VERTEX;

    for (int i = 0; i < graph->vertices[index1].count; ++i) {
        if (graph->vertices[index1].edges[i].to == index2)
            return DUPLICATE;
    }

    edge_connection(graph, index1, index2);
    return OKAY;
}

enum FuncResult delete_vertex(Graph *graph, int x, int y)
{
    int index;
    if ((index = find_vertex(graph, x, y)) == -1)
        return NOT_FOUND_VERTEX;

    free(graph->vertices[index].edges);
    for (int i = 0; i < graph->size; ++i) {
        if (i == index)
            continue;

        for (int j = 0; j < graph->vertices[i].count; ++j) {
            if (graph->vertices[i].edges[j].to == index) {
                correct_edge(graph, j, i);
                break;
            }
        }
        for (int j = 0; j < graph->vertices[i].count; ++j) {
            if (graph->vertices[i].edges[j].to > index)
                --graph->vertices[i].edges[j].to;

            if (graph->vertices[i].edges[j].from > index)
                --graph->vertices[i].edges[j].from;
        }
    }
    for (int i = index; i < graph->size - 1; ++i)
        swap_vertices(graph, i);

    graph->vertices[graph->size - 1].edges = NULL;
    --graph->size;
    return OKAY;
}

enum FuncResult delete_edge(Graph *graph, int x1, int y1, int x2, int y2)
{
    if (check_vertex_on_close(x1, y1, x2, y2) == FALSE)
        return VERTICES_NOT_CLOSE;

    int index1, index2;
    if ((index1 = find_vertex(graph, x1, y1)) == -1 || (index2 = find_vertex(graph, x2, y2)) == -1)
        return NOT_FOUND_VERTEX;

    for (int i = 0; i < graph->vertices[index1].count; ++i) {
        if (graph->vertices[index1].edges[i].to == index2) {
            correct_edge(graph, i, index1);
            return OKAY;
        }
    }
    return NOT_FOUND_PATH;
}

enum FuncResult change_type(Graph *graph, int x, int y, enum VertexType type)
{
    int index;
    if ((index = find_vertex(graph, x, y)) == -1)
        return NOT_FOUND_VERTEX;

    graph->vertices[index].type = type;
    return OKAY;
}

void print_graph(Graph *graph)
{
    for (int i = 0; i < graph->size; ++i)
        print_edges(graph, graph->vertices[i]);
}

void print_graph_file(FILE *file, Graph *graph)
{
	for (int i = 0; i < graph->size; ++i)
		print_edges_file(file, graph, graph->vertices[i]);
}

void destroy_graph(Graph *graph)
{
	if (graph == NULL)
		return;
		
    for (int i = 0; i < graph->size; i++) {
        free(graph->vertices[i].edges);
    }
    free(graph->vertices);
    free(graph);
}

enum FuncResult depth_first_search(Graph *graph, int x, int y, int *in)
{
    int index;
    if ((index = find_vertex(graph, x, y)) == -1 || graph->vertices[index].type != ENTER)
        return NOT_FOUND_VERTEX;

    DFS *array = (DFS *) malloc(sizeof(DFS) * graph->size);
    for (int i = 0; i < graph->size; ++i) {
        array[i].color = WHITE;
        array[i].pred = -1;
    }

    int time = 0;
    *in = -1;
    enum Bool result = FALSE;
    DFS_visit(graph, array, index, &time, &result, in);
    free(array);

    if (result == FALSE)
        return NOT_FOUND_PATH;

    return OKAY;
}

Bellman *BellmanFord(Graph *graph, int x1, int y1, int x2, int y2, int *index)
{
    int index1, index2;
    if ((index1 = find_vertex(graph, x1, y1)) == -1 || (index2 = find_vertex(graph, x2, y2)) == -1 ||
            graph->vertices[index1].type != ENTER || graph->vertices[index2].type != EXIT)
        return NULL;

    *index = index2;
    Bellman *array = (Bellman *) malloc(sizeof(Bellman) * graph->size);
    for (int i = 0; i < graph->size; ++i) {
        array[i].distance = INT_MAX / 2 - 1;
        array[i].pred = -1;
    }
    array[index1].distance = 0;

    for (int i = 1; i <= graph->size; ++i) {
        for (int j = 0; j < graph->size; ++j) {
            for (int k = 0; k < graph->vertices[j].count; ++k) {
                if (array[graph->vertices[j].edges[k].to].distance > array[j].distance + 1) {
                    array[graph->vertices[j].edges[k].to].distance = array[j].distance + 1;
                    array[graph->vertices[j].edges[k].to].pred = j;
                }
            }
        }
    }
    if (array[index2].pred == -1) {
        free(array);
        return NULL;
    }
    return array;
}
