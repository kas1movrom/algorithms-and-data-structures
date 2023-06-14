#include "dialog.h"

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <time.h>

#define eof -1

const char *graph_msgs[] = {
        "Successfully!\n",
        "Duplicate point!\n",
        "Vertices not close!\n",
        "Cant find vertex\n",
        "Cant found path\n"
};

int get_positive_int(int *value)
{
    int n;
    while ((n = scanf("%d", value)) != 1 || *value <= 0)
    {
        if (n == EOF)
        {
            return -1;
        }
        printf("Enter positive int value!\n");
        scanf("%*c");
    }
    return 1;
}

int get_number_type(int *type)
{
    int n;
    while ((n = scanf("%d", type)) != 1 || *type < 0 || *type > 2)
    {
        if (n == EOF)
        {
            return -1;
        }
        printf("Enter the correct number of release!\n");
        scanf("%*c");
    }
    return 1;
}

int dialog(const char *msgs[], int N)
{
    char *errmsg = "";
    int rc;
    int i, n;
    do
    {
        puts(errmsg);
        errmsg = "Choose the correct number of alternatives!";

        for(i = 0; i < N; ++i)
            puts(msgs[i]);
        puts("Choose showed alternatives -->");
        while ((n = scanf("%d", &rc)) != 1 || rc < 0 || rc > 12)
        {
            if (n == EOF)
            {
                rc = 0;
                break;
            }
            printf("\n%s\n", errmsg);
            n = scanf("%*c");
        }
        if(n == 0)
            rc = 0;
    } while(rc < 0 || rc >= N);
    return rc;
}

int dialog_dot_file(Graph *graph)
{
    char *filename = "file.dot";
    FILE *file = fopen(filename, "w");
    fprintf(file, "digraph {\n");
    create_dot_file(file, graph);
    fprintf(file, "}");
    fclose(file);
    system("dot -Tpng file.dot -o image.png");
    system("catimg image.png");
    return 1;
}

int dialog_spanning_tree(Graph *graph)
{
    printf("%s", graph_msgs[minimum_spanning_tree(graph)]);
    return 1;
}

int dialog_add_vertex(Graph *graph)
{
    int x, y, check1, check2;
    int type;

    printf("Enter point coordinates:\n>");
    check1 = get_positive_int(&x);
    if (check1 == -1)
        return eof;

    printf(">");
    check2 = get_positive_int(&y);
    if (check2 == eof)
        return eof;

    printf("Enter vertex type:\n\n");
    printf("0. Enter\n1. Exit\n2. Connect\n");
    check1 = get_number_type(&type);
    if (check1 == -1)
        return eof;

    printf("%s", graph_msgs[add_vertex(graph, x, y, type)]);
    return 1;
}

int dialog_add_edge(Graph *graph)
{
    int x1, y1, x2, y2, check;
    printf("Enter first point coordinates:\n>");
    check = get_positive_int(&x1);
    if (check == -1)
        return eof;

    printf(">");
    check = get_positive_int(&y1);
    if (check == eof)
        return eof;

    printf("Enter second point coordinates:\n>");
    check = get_positive_int(&x2);
    if (check == -1)
        return eof;

    printf(">");
    check = get_positive_int(&y2);
    if (check == eof)
        return eof;

    printf("%s", graph_msgs[add_edge(graph, x1, y1, x2, y2)]);
    return 1;
}

int dialog_delete_vertex(Graph *graph)
{
    int x, y, check1, check2;

    printf("Enter point coordinates:\n>");
    check1 = get_positive_int(&x);
    if (check1 == -1)
        return eof;

    printf(">");
    check2 = get_positive_int(&y);
    if (check2 == eof)
        return eof;

    printf("%s", graph_msgs[delete_vertex(graph, x, y)]);
    return 1;
}

int dialog_delete_edge(Graph *graph)
{
    int x1, y1, x2, y2, check;
    printf("Enter first point coordinates:\n>");
    check = get_positive_int(&x1);
    if (check == -1)
        return eof;

    printf(">");
    check = get_positive_int(&y1);
    if (check == eof)
        return eof;

    printf("Enter second point coordinates:\n>");
    check = get_positive_int(&x2);
    if (check == -1)
        return eof;

    printf(">");
    check = get_positive_int(&y2);
    if (check == eof)
        return eof;

    printf("%s", graph_msgs[delete_edge(graph, x1, y1, x2, y2)]);
    return 1;
}

int dialog_change_type(Graph *graph)
{
    int x, y, check1, check2;
    int type;

    printf("Enter point coordinates:\n>");
    check1 = get_positive_int(&x);
    if (check1 == -1)
        return eof;

    printf(">");
    check2 = get_positive_int(&y);
    if (check2 == eof)
        return eof;

    printf("Enter vertex type:\n\n");
    printf("0. Enter\n1. Exit\n2. Connect\n");
    check1 = get_number_type(&type);
    if (check1 == -1)
        return eof;

    printf("%s", graph_msgs[change_type(graph, x, y, type)]);
    return 1;
}

int dialog_print_graph(Graph *graph)
{
    printf("Yor graph:\n");
    print_graph(graph);
    return 1;
}


int dialog_DFS(Graph *graph)
{
    int x, y, check1, check2;

    printf("Enter point coordinates:\n>");
    check1 = get_positive_int(&x);
    if (check1 == -1)
        return eof;

    printf(">");
    check2 = get_positive_int(&y);
    if (check2 == eof){
        return eof;
	}
	int in = -1;
    printf("%s", graph_msgs[depth_first_search(graph, x, y, &in)]);
    return 1;
}

int dialog_Bellman(Graph *graph)
{
    int x1, y1, x2, y2, check;
    printf("Enter first point coordinates:\n>");
    check = get_positive_int(&x1);
    if (check == -1)
        return eof;

    printf(">");
    check = get_positive_int(&y1);
    if (check == eof)
        return eof;

    printf("Enter second point coordinates:\n>");
    check = get_positive_int(&x2);
    if (check == -1)
        return eof;

    printf(">");
    check = get_positive_int(&y2);
    if (check == eof)
        return eof;

    int index = 0;
    Bellman *array = BellmanFord(graph, x1, y1, x2, y2, &index);
    if (array == NULL)
    {
        printf("%s", graph_msgs[4]);
        return 0;
    }

    int pred = index, length = 0;
    printf ("Your path:\n");
    while (1)
    {
        print_vertex(graph->vertices[pred]);
        ++length;
        pred = array[pred].pred;
        if (pred == -1)
            break;

        printf(" <-- ");
    }
    printf("\nlength : %d\n", length - 1);
    free(array);
    return 1;
}

int dialog_dop_process(Graph *graph)
{
	char *filename = readline(">");
	srand(time(NULL));
	FILE *file = generate_file(filename);
	fclose(file);
	file = fopen(filename, "r");
	//destroy_graph(graph);

	//graph = create_graph();
	dop_process(file, graph);
	fclose(file);
	free(filename);
	return 0;
}
