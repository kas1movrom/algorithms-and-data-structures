#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"
#include "tree.h"

#define SUCCESS 1
#define FAIL 0

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

knot *create_element(unsigned int key, char *info)
{
    knot *element = (knot*)malloc(sizeof(knot));
    element->right = NULL;
    element->left = NULL;
    element->parent = NULL;
    element->detour = NULL;
    element->key = key;
    element->info = strdup(info);
    return element;
}

void destroy_element(knot *element)
{
    element->right = NULL;
    element->left = NULL;
    element->parent = NULL;
    element->detour = NULL;
    free(element->info);
    free(element);
}

int delete_tree(knot *ptr)
{
        if (ptr == NULL) {
                return SUCCESS;
        }
        delete_tree(ptr->left);
        delete_tree(ptr->right);
        destroy_element(ptr);
        return SUCCESS;
}


int get_cnt_key(knot *ptr, unsigned int key)
{
        int cnt = 0;
        while (1)
        {
                if (ptr->key == key) {
                        cnt++;
                }
                if (key < ptr->key) {
                        ptr = ptr->left;
                } else {
                        ptr = ptr->right;
                }
                if (ptr == NULL) {
                        break;
                }
        }
        return cnt;
}

int check_for_det(int key)
{
    int cnt = 0;
    while (key) {
        cnt++;
        key /= 10;
    }
    return cnt;
}

unsigned int find_min(knot *ptr)
{
    if (ptr == NULL) {
        return FAIL;
    }
    while (ptr->left != NULL) {
        ptr = ptr->left;
    }
    return ptr->key;
}

knot *find_next_element(knot *this)
{
    knot *result;
    if (this->right != NULL)
    {
        result = find_knot(this->right, find_min(this->right), 0);
        return result;
    }
    result = this->parent;
    while (result != NULL && this == result->right)
    {
        this = result;
        result = this->parent;
    }
    return result;
}

void set_detour_tree(tree *wood)
{
	knot *ptr = wood->root, *first = NULL;
	while (ptr != NULL)
	{
		first = ptr;
		if (ptr->left != NULL) {
			ptr = ptr->left;
		} else {
			ptr = ptr->right;
		}
	}
	wood->root->detour = first;
	knot *tmp = first, *next = NULL;
	while (tmp != wood->root)
	{
		if (tmp->parent->right == tmp || tmp->parent->right == NULL) {
			tmp->detour = tmp->parent;
		} else {
			next = tmp->parent->right;
			while (next != NULL)
			{
				tmp->detour = next;
				if (next->left != NULL) {
					next = next->left;
				} else {
					next = next->right;
				}
			}
		}
		tmp = tmp->detour;
	}
}

void test_detour(tree *wood)
{
	knot *ptr = wood->root->detour;
	while (ptr != wood->root)
	{
		printf("%d ", ptr->key);
		ptr = ptr->detour;
	}
}

int find_this_release(tree *wood, knot *element)
{
	knot *ptr = wood->root;
	int cnt = 0;
	while (ptr != element)
	{
		if (ptr->key == element->key)
			++cnt;

		if (element->key < ptr->key) {
			ptr = ptr->left;
		} else {
			ptr = ptr->right;
		}
	}
	return cnt;
}
