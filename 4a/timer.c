#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <readline/readline.h>

#include "func.h"
#include "tree.h"
#include "dialog.h"


#define CNT 19

typedef struct knot {
	unsigned int key;
	struct knot *right;
	struct knot *left;
	struct knot *detour;
	struct knot *parent;
} knot;


typedef struct tree {
	knot *root;
} tree;

typedef double (*t_func) (tree *, int);


knot *create_element_timer();
knot *delete_element_timer(tree *wood, knot *this);

void destr_element_timer(knot *element)
{
	element->right = NULL;
	element->left = NULL;
	element->parent = NULL;
	element->detour = NULL;
	free(element);
}

int delete_tree_time(knot *ptr)
{
	if (ptr == NULL) {
		return 1;
	}
	delete_tree_time(ptr->left);
	delete_tree_time(ptr->right);
	destr_element_timer(ptr);
	return 1;
}

void destr_tree_timer(tree *wood)
{
	if (tree_is_empty(wood)) {
		free(wood);
	} else {
		delete_tree_time(wood->root);
		free(wood);
	}
}

double timer_add(tree *wood, int cnt)
{
	clock_t start, stop;
	start = clock();
	for (int i = 0; i < cnt; i++) {
		add_knot(wood, create_element_timer());
	}
	stop = clock();
	return stop - start;
}

double timer_delete_el(tree *wood, int cnt)
{
	knot *element;
	clock_t start, stop;
	int key;
	start = clock();
	for (int i = 0; i < cnt; i++) {
		key = rand();
		element = find_knot(wood->root, (key < 0) ? -key : key, 0);
		if (element != NULL)
			delete_element_timer(wood, element);
	}
	stop = clock();
	return stop - start;	
}

double timer_find(tree *wood, int cnt)
{
	int key;
	clock_t start, stop;
	start = clock();
	for (int i = 0; i < cnt; ++i) {
		key = rand();
		find_knot(wood->root, (key < 0) ? (-key) : key, 0);
	}
	stop = clock();
	return stop - start;
}

knot *create_element_timer()
{
	knot *element = (knot*)malloc(sizeof(knot));
	element->key = rand();
	return element;
}

knot *delete_element_timer(tree *wood, knot *this)
{
	knot *result = NULL;
	if (this->right == NULL || this->left == NULL) {
		result = this;
	} else {
		result = find_next_element(this);
	}
	knot *ptr, *parent;
	if (result->left != NULL) {
		ptr = result->left;
	} else {
		ptr = result->right;
	}
	parent = result->parent;
	if (ptr != NULL) {
		ptr->parent = parent;
	}
	if (parent == NULL) {
		wood->root = ptr;
	}

	if (parent->left == result) {
		parent->left = ptr;
	} else {
		parent->right = ptr;
	}

	if (result != this) {
		this->key = result->key;
	}
	return result;
}

int main()
{


	t_func func[3] = {timer_add, timer_delete_el, timer_find};
	int arr[19] = {1000, 2000, 3000, 5000, 10000, 15000, 
					30000, 50000, 60000, 70000, 90000, 110000, 
					130000, 150000, 200000, 250000, 500000, 1000000, 5000000};

	double *time_vector = (double*)malloc(sizeof(double) * 19);
	
	srand(time(NULL));

	int ch;
	printf("1. Time add\n2. Time delete\n3. Time find\n");
	printf("Choose one of the showed alternatives:\n");
	scanf("%d", &ch);
	for (int i = 0; i < 19; ++i) {
		
		double time = 0.0;

		for (int k = 0; k < 10; k++){
			tree *wood = create_tree();
			for (int j = 0; j < arr[i]; ++j) {
				knot *element = create_element_timer();
				add_knot(wood, element);
			}
			
			time += (func[ch](wood, 1000) / CLOCKS_PER_SEC);
			//printf("%d,%f\n", arr[i], time / CLOCKS_PER_SEC);

			destr_tree_timer(wood);
		}

		printf("%d,%f\n", arr[i], time / 10);
		time_vector[i] = time / 10;

	}

	char *filename = readline(">");
	FILE *file = fopen(filename, "w");

	for (int i = 0; i < 19; ++i) {
		fprintf(file, "%d,%f\n", arr[i], time_vector[i]);
	}
	fclose(file);
	free(filename);
	free(time_vector);
	
	return 0;
}
