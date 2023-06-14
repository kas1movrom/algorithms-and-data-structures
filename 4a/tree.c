#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "func.h"

#define SUCCESS 1
#define FAIL 0
#define QUOTES 34

typedef struct knot {
	unsigned int key;
	char *info;

	struct knot *right;
	struct knot *left;
	struct knot *parent;
	struct knot *detour;
} knot;

typedef struct tree {
	knot *root;
} tree;

int tree_is_empty(tree *wood)
{
    if (wood->root == NULL) {
        return 1;
    }
    return 0;
}

tree *create_tree()
{
    tree *wood = (tree*)malloc(sizeof(tree));
    wood->root = NULL;
    return wood;
}

void destroy_tree(tree *wood)
{
        if (tree_is_empty(wood)) {
                free(wood);
        } else {
                delete_tree(wood->root);
                free(wood);
        }
}

int add_knot(tree *wood, knot *element)
{
    if (tree_is_empty(wood))
    {
        wood->root = element;
        return SUCCESS;
    }
    knot *ptr = wood->root;
    knot *par = NULL;
    while (ptr != NULL)
    {
        par = ptr;
        if (element->key < ptr->key) {
            ptr = ptr->left;
        } else {
            ptr = ptr->right;
        }
    }
    if (element->key < par->key) {
        par->left = element;
    } else {
        par->right = element;
    }
    element->parent = par;
    return SUCCESS;
}

int print_tree(knot *ptr)
{
    if (ptr == NULL) {
        return SUCCESS;
    }
    print_tree(ptr->left);
    print_tree(ptr->right);
    printf("%d. %s\n", ptr->key, ptr->info);
    return SUCCESS;
}

int detour_tree(knot *ptr, int cnt)
{
     if (ptr == NULL) {
         return SUCCESS;
     }
     detour_tree(ptr->left, cnt);
     detour_tree(ptr->right, cnt);
     if (check_for_det(ptr->key) == cnt) {
         printf("%d. %s\n", ptr->key, ptr->info);
     }
     return SUCCESS;
}

knot *find_knot(knot *ptr, unsigned int key, int release) {
    int cnt = 0;
    while (ptr != NULL)
    {
        if (ptr->key == key)
        {
            if (cnt == release) {
                return ptr;
            }
            cnt++;
        }
        if (key < ptr->key) {
            ptr = ptr->left;
        } else {
            ptr = ptr->right;
        }
    }
    return NULL;
}

knot *delete_element(tree *wood, knot *this)
{
     knot *result;
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
         free(this->info);
         this->info = strdup(result->info);
     }
     return result;
}

void printf_tree(knot *element, int p)
{
     if (element != NULL)
     {
         printf_tree(element->right, p + 3);
         for (int i = 0; i < p; ++i)
         {
             printf(" ");
         }
         printf("%3d. %s\n", element->key, element->info);
         printf_tree(element->left, p + 3);
     }
     printf("\n");
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
    } while (n > 0);
    return ptr;
}

int create_dot_file(FILE *file, knot *ptr, tree *wood)
{
	if (ptr == NULL)
		return 0;

	int release = find_this_release(wood, ptr);
	if (ptr->left != NULL)
		fprintf(file, "\t%c%d-%d%c -> %c%d-%d%c\n", QUOTES, ptr->key, release, QUOTES, QUOTES,
			ptr->left->key, find_this_release(wood, ptr->left), QUOTES);

	if (ptr->right != NULL)
		fprintf(file, "\t%c%d-%d%c -> %c%d-%d%c\n", QUOTES, ptr->key, release, QUOTES, QUOTES, 
			ptr->right->key, find_this_release(wood, ptr->right), QUOTES);

	create_dot_file(file, ptr->left, wood);
	create_dot_file(file, ptr->right, wood);
	return 1;		
		
}
