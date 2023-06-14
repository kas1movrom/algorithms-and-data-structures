#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

typedef struct knot {
	struct knot *left;
	struct knot *right;
	struct knot *parent;

	int key;
	unsigned int count;
} knot;

typedef struct tree {
	knot *root;
} tree;

tree *create_tree()
{
    tree *wood = (tree*)malloc(sizeof(tree));
    wood->root = NULL;
    return wood;
}

int add_knot(tree *wood, knot *element)
{
	if (wood->root == NULL)
	{
		wood->root = element;
		return 1;
	}
	knot *ptr = wood->root;
	knot *par = NULL;
	while (ptr != NULL)
	{
		par = ptr;
		if (element->key < par->key)
		{
			ptr = ptr->left;
		}
		if (element->key == par->key)
		{
			par->count = par->count + 1;
			return 0;
		}
		if (element->key > par->key)
		{
			ptr = ptr->right;
		}
	}
	if (element->key < par->key) {
		par->left = element;
	} else {
		par->right = element;
	}
	element->parent = par;
	return 1;
}

knot *create_element(int key)
{
    knot *element = (knot*)malloc(sizeof(knot));
    element->right = NULL;
    element->left = NULL;
    element->parent = NULL;
    element->key = key;
    element->count = 1;
    return element;
}


void process(FILE *file, tree *wood)
{
	int target = 0;
	char er_buf;
	fseek(file, target, SEEK_SET);
	while (!feof(file))
	{
		fseek(file, target, SEEK_SET);
		knot *element = create_element(0);
		fread(&(element->key), sizeof(int), 1, file);
		if (!add_knot(wood, element)) {
			free(element);
		}
		target = ftell(file);
		fread(&er_buf, sizeof(char), 1, file);
	}
}

int delete_tree(knot *ptr)
{
	if (ptr == NULL)
		return 1;

	delete_tree(ptr->left);
	delete_tree(ptr->right);
	free(ptr);
	return 1;
}

void destroy_tree(tree *wood)
{
	if (wood->root == NULL) {
		free(wood);
	} else {
		delete_tree(wood->root);
		free(wood);
	}
}

int process1(FILE *file, knot *ptr)
{
	if (ptr == NULL)
		return 1;

	process1(file, ptr->left);
	process1(file, ptr->right);
	fprintf(file, "%d --- %d\n", ptr->key, ptr->count);
	return 1;
}

void process0(FILE *file, int cnt)
{
	int value;
	while (cnt > 0)
	{
		scanf("%d", &value);
		fwrite(&value, sizeof(int), 1, file);
		cnt--;
	}
}

int main() 
{
	tree *wood = create_tree();
	char *file_byn = readline(">");
	FILE *file_in = fopen(file_byn, "rb");

	
	if (file_in == NULL) {
		free(file_byn);
		return 1;
	}
	process(file_in, wood);
	fclose(file_in);
	free(file_byn);

	char *file_txt = readline(">");
	FILE *file_out = fopen(file_txt, "w");
	process1(file_out, wood->root);
	fclose(file_out);
	free(file_txt);
	destroy_tree(wood);
	
	return 0;	
}
