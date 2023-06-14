#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

int main()
{
	printf("Enter the file name:\n");
	char *filename = readline(">");
	printf("Enter the count of numbers, whom you will write in file:\n");
	int cnt;
	scanf("%d", &cnt);

	printf("Enter the %d numbers\n", cnt);

	int *arr = (int*)malloc(sizeof(int) * cnt);
	for (int i = 0; i < cnt; ++i) {
		scanf("%d", arr + i);
	}

	FILE *file = fopen(filename, "wb");
	for (int i = 0; i < cnt; ++i) {
		fwrite(&(arr[i]), sizeof(int), 1, file);
	}

	fclose(file);
	free(filename);
	free(arr);

	return 0;
	
}
