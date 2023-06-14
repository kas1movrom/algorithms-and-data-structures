#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <readline/readline.h>

#include "func.h"

typedef double (*time_func) (tree *, int);

char *generate_str()
{
	char *str = (char*)malloc(sizeof(char) * 4);
	for (int i = 0; i < 3; ++i) {
		str[i] = (char)((rand() % 92) + 33);
	}
	str[3] = '\0';
	return str;
}

double timer_add(tree *wood, int cnt)
{
	clock_t start, stop;
	start = clock();
	char *key, *info;
	for (int i = 0; i < cnt; ++i) {
		key = generate_str();
		info = generate_str();
		insert_tree(wood, key, info);
		free(key);
		free(info);
	}
	stop = clock();
	return stop - start;
}

double time_delete(tree *wood, int cnt)
{
	clock_t start, stop;
	int release;
	char *key;
	item *element;
	start = clock();
	for (int i = 0; i < cnt; ++i) {
		key = generate_str();
		element = tree_search(wood->root, key);
		if (element == NULL) {
			free(key);
			element = NULL;
			continue;
		}
		release = get_releases(element);
		if (release != 0) {
			remove_release(release, element);
			free(key);
			element = NULL;
			continue;
		}
		free(key);
	}
	stop = clock();
	return stop - start;
}

double timer_find(tree *wood, int cnt)
{
	char *key;
	clock_t start, stop;
	start = clock();
	for (int i = 0; i < cnt; ++i) {
		key = generate_str();
		tree_search(wood->root, key);
		free(key);
	}
	stop = clock();
	return stop - start;
}

int main()
{
	time_func func[3] = {timer_add, time_delete, timer_find};
	
	int arr[19] = {1000, 2000, 3000, 5000, 10000, 15000, 30000, 50000,
					60000, 70000, 90000, 110000, 130000, 150000, 200000,
					250000, 500000, 750000, 1000000};

	double *time_vec = (double*)malloc(sizeof(double) * 19);
	srand(time(NULL));

	int ch;
	printf("1. Time add\n2. Time delete\n3. Time find\n");
	printf("Choose one of the showed alternatives\n");
	scanf("%d", &ch);

	char *key, *info;
	for (int i = 0; i < 19; ++i) {
		double time = 0.0;

		for (int j = 0; j < 10; ++j) {
			tree *wood = create_tree();
			for (int k = 0; k < arr[i]; ++k) {
				key = generate_str();
				info = generate_str();
				insert_tree(wood, key, info);
				free(key);
				free(info);
			}

			time += (func[ch](wood, 1000) / CLOCKS_PER_SEC);

			delete_tree(wood->root);
			free(wood);
		}

		printf("%d,%f\n", arr[i], time / 10);
		time_vec[i] = time / 10;
	}


	char *filename = readline(">");
	FILE *file = fopen(filename, "w");
	for (int i = 0; i < 19; ++i) {
		fprintf(file, "%d,%f\n", arr[i], time_vec[i]);
	}
	fclose(file);
	free(filename);
	free(time_vec);
	return 0;
}
