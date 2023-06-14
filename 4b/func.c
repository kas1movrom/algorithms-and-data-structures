#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

#define sep ";"

knot *create_knot()
{
	knot *element = (knot*)malloc(sizeof(knot));
	element->size = 0;
	element->arr = (item**)malloc(sizeof(item*) * (2 * EXT - 1));
	for (int i = 0; i < 2 * EXT - 1; ++i) {
		element->arr[i] = NULL;
	}
	element->child = (knot**)malloc(sizeof(knot*) * 2 * EXT);
	for (int i = 0; i < 2 * EXT; ++i) {
		element->child[i] = NULL;
	}
	element->parent = NULL;
	return element;
}

node *create_node(char *info, item *list)
{
	node *element = (node*)malloc(sizeof(node));
	element->next = list->head;
	element->info = strdup(info);
	return element;
}

bool knot_is_sheet(knot *ptr)
{
	for (int i = 0; i < 2 * EXT; ++i) {
		if (ptr->child[i] != NULL)
		{
			return 0;
		}
	}
	return 1;
}

bool knot_full(knot *ptr)
{
	return ptr->size == 2 * EXT - 1;
}

bool add_node(char *info, item *list)
{
	node *element = create_node(info, list);
	list->head = element;
	return 1;
}

void sort_sheet(knot *sheet)
{
	if (sheet == NULL || sheet->arr == NULL) {
		return;
	}
	for (int i = 0; i < sheet->size; ++i) {
		for (int j = 0; j < sheet->size - 1; ++j) {
			if (sheet->arr[j + 1] == NULL) {
				continue;
			}
			if ((sheet->arr[j] == NULL) 
				|| strcmp(sheet->arr[j]->key, sheet->arr[j + 1]->key) > 0)
			{
				item *tmp = sheet->arr[j];
				sheet->arr[j] = sheet->arr[j + 1];
				sheet->arr[j + 1] = tmp;
			}
		}
	}
}

item *create_item(char *key)
{
	item *element = (item*)malloc(sizeof(item));
	element->head = NULL;
	element->key = strdup(key);
	return element;
}

bool add_key(knot *ptr, char *key, char *info)
{
	ptr->arr[ptr->size] = create_item(key);
	add_node(info, ptr->arr[ptr->size]);
	ptr->size = ptr->size + 1;
	sort_sheet(ptr);
	return 1;
	
}

void tree_split(knot *element, int pos)
{
	knot *target = element->child[pos];
	int cnt = target->size, j = 0;
	for (int i = 2 * EXT - 1; i > pos + 1; --i)
	{
		element->child[i] = element->child[i - 1];
		element->arr[i - 1] = element->arr[i - 2];
	}

	/*element->arr[0] = NULL;
	element->child[1] = 0*/

	knot *new = create_knot();
	for (int i = cnt / 2 + 1; i < cnt; ++i)	{
		new->arr[j] = target->arr[i];
		target->arr[i] = NULL;
		new->child[j] = target->child[i];
		target->child[i] = NULL;
		--target->size;
		++new->size;
		++j;
	}

	new->child[j] = target->child[cnt];
	target->child[cnt] = NULL;

	
	element->child[pos + 1] = new;
	element->arr[pos] = target->arr[cnt / 2];
	target->arr[cnt / 2] = NULL;
	--target->size;
	++element->size;

	sort_sheet(element);
	sort_sheet(target);
	sort_sheet(new);
}

bool insert_nonfull(knot *ptr, char *key, char *info)
{
	knot *target;
	while (!knot_is_sheet(ptr))
	{
		int index;
		if (strcmp(key, ptr->arr[0]->key) < 0)
		{
			index = 0;
			goto under_the_tree;
		}
		if (strcmp(key, ptr->arr[ptr->size - 1]->key) > 0)
		{
			index = ptr->size;
			goto under_the_tree;
		}
		for (int i = 0; i < ptr->size - 1; ++i) {
			if (strcmp(key, ptr->arr[i]->key) > 0 && strcmp(key, ptr->arr[i + 1]->key) < 0)
			{
				index = i + 1;
				break;
			}
		}
		under_the_tree : target = ptr->child[index];
		if (knot_full(target))
		{
			tree_split(ptr, index);
			if (strcmp(key, ptr->arr[index]->key) > 0)
			{
				target = ptr->child[index + 1];
			}
		}
		ptr = target;
	}

	add_key(ptr, key, info);
	sort_sheet(ptr);
	return 1;
}

void print_item(item *element)
{
	if (element == NULL) {
		return;
	}
	printf("%s:", element->key);
	node *ptr = element->head;
	while (ptr != NULL)
	{
		printf(" %s ", ptr->info);
		ptr = ptr->next;
	}
	printf("\n");
}

int print_detour(knot *ptr, char *key)
{
	if (key == NULL)
	{
		for (int i = 0; i < ptr->size; ++i) {
			print_item(ptr->arr[i]);
		}
		return 1;
	}
	
	for (int i = 0; i < ptr->size; ++i) {
		if (strcmp(ptr->arr[i]->key, key) < 0)
		{
			print_item(ptr->arr[i]);
		}
	}
	return 1;
}

item *min_search(knot *ptr)
{
	while (ptr->child[0] != NULL)
	{
		ptr = ptr->child[0];
	}
	return ptr->arr[0];
}

item *max_search(knot *ptr)
{
	while (ptr->child[ptr->size] != NULL)
	{
		ptr = ptr->child[ptr->size];
	}
	return ptr->arr[ptr->size - 1];
}

int generate_val(char *str)
{
	int sum = 0;
	for (int i = 0; i < (int)strlen(str); ++i) {
		sum += (int)str[i];
	} 
	return sum;
}

void move_pointer(knot *ptr, item *element)
{
	ptr->arr[ptr->size] = element;
	++ptr->size;
}

void destroy_knot(knot *ptr)
{
	free(ptr->arr);
	free(ptr->child);
	free(ptr);
}

void process_tree(tree *wood)
{
	if (wood->root->size == 1 && !knot_is_sheet(wood->root) && wood->root->child[0]->size == EXT - 1 
		&& wood->root->child[1]->size == EXT - 1)
	{
		knot *root = wood->root;
		knot *left = wood->root->child[0];
		knot *right = wood->root->child[1];
		
		move_pointer(left, wood->root->arr[0]);
		wood->root->arr[0] = NULL;
		for (int i = 0; i < EXT - 1; ++i) {
			move_pointer(left, right->arr[i]);
			right->arr[i] = NULL;
		}

		for (int i = 0; i < EXT; ++i) {
			left->child[EXT + i] = right->child[i];
			right->child[i] = NULL;
		}

		wood->root->child[0] = NULL;
		wood->root->child[1] = NULL;
		wood->root = left;

		destroy_knot(root);
		destroy_knot(right);
	}
}

int choose_index(knot *ptr, char *key)
{
	for (int i = 0; i < ptr->size; ++i) {
		if (strcmp(key, ptr->arr[i]->key) <= 0)
		{
			return i;
		}
	}
	if (ptr->size == 2 * EXT - 1)
	{
		return 2 * EXT - 1; // было 2 * EXT - 2
	}
	return ptr->size;
}

int get_releases(item *list)
{
	node *ptr = list->head;
	int cnt = 0;
	while (ptr != NULL)
	{
		++cnt;
		ptr = ptr->next;
	}
	return cnt;
}

bool knot_contains(knot *ptr, char *key)
{
	for (int i = 0; i < ptr->size; ++i) {
		if (strcmp(key, ptr->arr[i]->key) == 0) {
			return i;
		}
	}
	return -1;
}

void remove_item(knot *ptr, int index)
{
	item *target = ptr->arr[index];
	free(target->key);
	free(target->head->info);
	free(target->head);
	free(ptr->arr[index]);
	ptr->arr[index] = NULL;
	sort_sheet(ptr);
	--ptr->size;
}

bool contains_ext(knot *ptr, int index)
{
	if (index == 2 * EXT - 1) {
		return (ptr->child[index - 1] != NULL && ptr->child[index - 1]->size >= EXT);
	}

	if (!index) {
		return (ptr->child[index + 1] != NULL && ptr->child[index + 1]->size >= EXT);
	} else {
		return ((ptr->child[index + 1] != NULL && ptr->child[index + 1]->size >= EXT) ||
			(ptr->child[index - 1] != NULL && ptr->child[index - 1]->size >= EXT));
	}
}

bool neigh_contains(knot *ptr, int index)
{
	if (index == 2 * EXT - 1 || index == ptr->size) {
		return (ptr->child[index - 1] != NULL && ptr->child[index - 1]->size == EXT - 1);
	}	

	if (!index) {
		return (ptr->child[index + 1] != NULL && ptr->child[index + 1]->size == EXT - 1);
	} else {
		return ((ptr->child[index + 1] != NULL && ptr->child[index + 1]->size == EXT - 1) ||
			(ptr->child[index - 1] != NULL && ptr->child[index - 1]->size == EXT - 1));		
	}
}

knot *process1(knot *ptr, int index)
{
	knot *target = ptr->child[index];
	printf("size:%d, contains:%d\n", target->size, contains_ext(ptr, index));
	if (target->size == EXT - 1 && contains_ext(ptr, index)) { // нужно узнать кто именно сосед
		knot *neigh;
		printf("choose block1\n");
		/*if (index == 2 * EXT - 1) {
			neigh = ptr->child[index - 1];
			item *key_delimeter = ptr->arr[index - 1];
			move_pointer(target, key_delimeter);
			ptr->arr[index - 1]
		}*/
		
		if (index != 2 * EXT - 1 && ptr->child[index + 1] != NULL && ptr->child[index + 1]->size >= EXT) {
			neigh = ptr->child[index + 1];
			item *key_delimeter = ptr->arr[index];
			move_pointer(target, key_delimeter);
			ptr->arr[index] = neigh->arr[0];
			neigh->arr[0] = NULL;
			for (int i = 0; i < neigh->size - 1; ++i) {
				neigh->arr[i] = neigh->arr[i + 1];
			}
			neigh->arr[neigh->size - 1] = NULL;
			knot *resurs = neigh->child[0];
			for (int i = 0; i < neigh->size; ++i) {
				neigh->child[i] = neigh->child[i + 1];
			}
			neigh->child[neigh->size] = NULL;
			target->child[target->size] = resurs;
			--neigh->size;
		} else {
			neigh = ptr->child[index - 1];
			item *key_delimeter = ptr->arr[index - 1];
			for (int i = target->size; i > 0; --i) {
				target->arr[i] = target->arr[i - 1];
			}
			target->arr[0] = NULL;
			for (int i = target->size + 1; i > 0; --i) {
				target->child[i] = target->child[i - 1];
			}
			target->child[0] = NULL;
			target->arr[0] = key_delimeter;
			++target->size;
			ptr->arr[index - 1] = neigh->arr[neigh->size - 1];
			neigh->arr[neigh->size - 1] = NULL;
			target->child[0] = neigh->child[neigh->size];
			neigh->child[neigh->size] = NULL;
			--neigh->size;
		}
	} else if (target->size == EXT - 1 && neigh_contains(ptr, index)) {
		printf("choose if 2\n");
		
		if (index == 2 * EXT - 1 || index == ptr->size) {
			item *key_delimeter = ptr->arr[index - 1];
			int size = ptr->child[index - 1]->size;
			ptr->child[index] = NULL;
			--ptr->size;
			move_pointer(ptr->child[index - 1], key_delimeter);
			ptr->arr[index - 1] = NULL;
			for (int i = 0; i < target->size; ++i) {
				move_pointer(ptr->child[index - 1], target->arr[i]);
				target->arr[i] = NULL;
			}

			for (int i = 0; i < target->size + 1; ++i) {
				ptr->child[index - 1]->child[size + 1 + i] = target->child[i];
				target->child[i] = NULL;
			}

			destroy_knot(target);
			sort_sheet(ptr->child[index - 1]);
			target = ptr->child[index - 1];
			goto end;
			
		}
	
		item *key_delimeter = ptr->arr[index];
		move_pointer(target, key_delimeter);
		for (int i = index; i < ptr->size - 1; ++i) {
			ptr->arr[i] = ptr->arr[i + 1];
		}
		ptr->arr[ptr->size - 1] = NULL;
		knot *next = ptr->child[index + 1];

		target->child[target->size] = next->child[0];
		
		move_pointer(target, next->arr[0]);
		target->child[target->size] = next->child[1];
		next->arr[0] = NULL;
		next->child[0] = NULL;
		next->child[1] = NULL;

		destroy_knot(next);
		
		for (int i = index + 1; i < ptr->size; ++i) {
			ptr->child[i] = ptr->child[i + 1];
		}
		ptr->child[ptr->size] = NULL;
		--ptr->size;
	}	
	end : return target;
}

knot *search_previous(knot *ptr, int *index)
{
	while (ptr->child[ptr->size] != NULL)
	{
		ptr = ptr->child[ptr->size];
	}
	*index = ptr->size - 1;
	return ptr;
}

knot *search_next(knot *ptr, int *index)
{
	 while (ptr->child[0] != NULL)
	 {
	 	ptr = ptr->child[0];
	 }
	 *index = 0;
	 return ptr;
}

knot *process2(knot *ptr, int index)
{
	knot *left = ptr->child[index];
	knot *right = ptr->child[index + 1];
	if (left != NULL && left->size >= EXT) {
		int cnt = 0;
		knot *previous = search_previous(left, &cnt);
		item *tmp = previous->arr[cnt];
		previous->arr[cnt] = ptr->arr[index];
		ptr->arr[index] = tmp;
		return left;
	} else if (right != NULL && right->size >= EXT) {
		int cnt = 0;
		knot *next = search_next(right, &cnt);
		item *tmp = next->arr[cnt];
		next->arr[cnt] = ptr->arr[index];
		ptr->arr[index] = tmp;
		return right;
	} else {
		 move_pointer(left, ptr->arr[index]);
		 int size = left->size;
		 for (int i = 0; i < right->size; ++i) {
		 	move_pointer(left, right->arr[i]);
		 	right->arr[i] = NULL;
		 }
		 for (int i = 0; i < right->size + 1; ++i) {
		 	left->child[size + i] = right->child[i];
		 	right->child[i] = NULL;
		 }
		 destroy_knot(right);
		 for (int i = index; i < ptr->size - 1; ++i) {
		 	ptr->arr[i] = ptr->arr[i + 1];
		 }
		 ptr->arr[ptr->size - 1] = NULL;
		 for (int i = index + 1; i < ptr->size; ++i) {
		 	ptr->child[i] = ptr->child[i + 1];
		 }
		 ptr->child[ptr->size] = NULL;
		 --ptr->size;
		 return left;
	}
}

char *make_print(knot *ptr, char *result, char *prefix, char *children_prefix, int *len)
{
	if (ptr == NULL) {
		return result;
	}
	if (*len == 0) {
		memcpy(result, "0\n", 2);
		*len += 2;
	} else {
		memcpy(result + *len, prefix, strlen(prefix));
		*len += strlen(prefix);
		for (int i = 0; i < ptr->size; ++i) {
			if (ptr->arr[i] == NULL || ptr->arr[i]->key == NULL) {
				continue;
			}
			memcpy(result + *len, ptr->arr[i]->key, strlen(ptr->arr[i]->key));
			*len += strlen(ptr->arr[i]->key);
			memcpy(result + *len, " ", 1);
			*len += 1;
		}
		/*memcpy(result + *len, ptr->arr[index]->key, strlen(ptr->arr[index]->key));
		*len += strlen(ptr->arr[index]->key);*/
		memcpy(result + *len, "\n", 1);
		*len += 1;
	}

	for (int i = 0; i <= ptr->size; ++i) {
		if (i == ptr->size) {
			int child_len = strlen(children_prefix);
			char *first = malloc(sizeof(char) * (child_len + strlen("^-- ") + 1));
			memcpy(first, children_prefix, child_len);
			memcpy(first + child_len, "^-- ", strlen("^-- ") + 1);

			char *second = malloc(sizeof(char) * (child_len + strlen("    ") + 1));
			memcpy(second, children_prefix, child_len);
			memcpy(second + child_len, "    ", strlen("    ") + 1);
			
			result = make_print(ptr->child[i], result, first, second, len);

			free(first);
			free(second);
		} else {
			int child_len = strlen(children_prefix);
				
			char *first = malloc(sizeof(char) * (child_len + strlen("|-- ") + 1));
			memcpy(first, children_prefix, child_len);
			memcpy(first + child_len, "|-- ", strlen("|-- ") + 1);

			char *second = malloc(sizeof(char) * (child_len + strlen("|   ") + 1));
			memcpy(second, children_prefix, child_len);
			memcpy(second + child_len, "|   ", strlen("|   ") + 1);			
			
			result = make_print(ptr->child[i], result, first, second, len);

			free(first);
			free(second);
		}
	}
	return result;
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

char *make_shield(char *str)
{
	return strdup(str);
}

int delete_item(item *element)
{
	if (element == NULL) {
		return 0;
	}
	node *ptr = element->head, *next;
	while (ptr != NULL)
	{
		next = ptr->next;
		free(ptr->info);
		free(ptr);
		ptr = next;
	}
	free(element->key);
}

void delete_knot(knot *ptr)
{
	for (int i = 0; i < ptr->size; ++i) {
		delete_item(ptr->arr[i]);
		free(ptr->arr[i]);
		ptr->arr[i] = NULL;
	}
	for (int i = 0; i < ptr->size + 1; ++i) {
		ptr->child[i] = NULL;
	}
}

char *create_digraph_item(knot *ptr)
{
	if (ptr->arr == NULL) {
		return NULL;
	}
	int len = 0, cur_len;
	for (int i = 0; i < ptr->size; ++i) {
		if (ptr->arr[i] != NULL && ptr->arr[i]->key != NULL)
			len += (int)strlen(ptr->arr[i]->key);
	}
	len += ptr->size;
	//printf("len:%d\n", len);
	char *result = (char*)malloc(sizeof(char) * len);
	len = 0;
	for (int i = 0; i < ptr->size; ++i) {
		if (ptr->arr[i] != NULL && ptr->arr[i]->key != NULL) {
			cur_len = (int)strlen(ptr->arr[i]->key);
			//printf("\ncur len :%d\n", cur_len);
			memcpy(result + len, ptr->arr[i]->key, cur_len);
			len += cur_len;
			if (i == ptr->size - 1) {
				break;	
			}
			memcpy(result + len, "-", 1);
			len++;
		}
	} 
	len++;
	//printf("end len:%d\n", len);
	result[len - 1] = '\0';
	//printf("result :%s\n", result);
	return result;
}

int create_dot_file(FILE *file, knot *ptr)
{
	if (ptr == NULL || ptr->size == 0) {
		return 0;
	}

	char *element = create_digraph_item(ptr);
	//printf("element : %s\n", element);
	for (int i = 0; i < ptr->size + 1; ++i) {
		if (ptr->child[i] != NULL && ptr->child[i]->size != 0) 
		{
			char *this = create_digraph_item(ptr->child[i]);
			if (this == NULL) {
				continue;
			}
			fprintf(file, "\t%c%s%c -> %c%s%c\n", 34, element, 34, 34, this, 34);
			free(this);
		}
	}
	free(element);

	for (int i = 0; i < ptr->size + 1; ++i) {
		create_dot_file(file, ptr->child[i]);
	} 
	return 1;
}

int count_number(int value)
{
	int cnt = 0;
	if (value == 0) {
		return 1;
	}
	while (value != 0)
	{
		++cnt;
		value /= 10;
	}
	return cnt;
}

char *its(int value)
{
	int cnt = count_number(value), index = cnt;
	char *result = (char*)malloc(sizeof(char) * (cnt + 1));
	if (value == 0) {
		result[0] = '0';
		result[1] = '\0';
		goto ret;
	}
	while (value != 0)
	{
		int p = value % 10;
		result[cnt - 1] = '0' + p;
		--cnt;
		value /= 10;
	}
	result[index] = '\0';
	ret : return result;
}

char *create_info(char *filename, int cnt, int offset)
{
	int len = 0, cur_len = 0;
	char *count = its(cnt), *off = its(offset);
	char *result = (char*)malloc(sizeof(char) * ((int)strlen(filename) + count_number(cnt) + 
		count_number(offset) + 3));
		
	cur_len = (int)strlen(filename);
	memcpy(result + len, filename, cur_len);
	len += cur_len;
	memcpy(result + len, sep, 1);
	len++;
	
	cur_len = (int)strlen(count);
	memcpy(result + len, count, cur_len);
	len += cur_len;
	memcpy(result + len, sep, 1);
	len++;

	cur_len = (int)strlen(off);
	memcpy(result + len, off, cur_len);
	len += cur_len;

	result[len] = '\0';
	free(count);
	free(off);
	return result;
}
