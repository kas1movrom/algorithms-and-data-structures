#include <stdio.h>
#include <stdlib.h>

typedef struct line {
    int cnt;
    int *data;
} line;

typedef struct Matrix {
    int cnt;
    line *data;
} Matrix;

Matrix *init_matrix(int cnt_matrix) {
    Matrix *mtr = (Matrix*)malloc(sizeof(Matrix));
    mtr->cnt = cnt_matrix;
    mtr->data = (line*)malloc(sizeof(line) * (mtr->cnt));
    return mtr;
}

void bubbleSort(int *arr, line object) {
    for (int j = 0; j < object.cnt; j++) {
        for (int k = 0; k < object.cnt - 1; k++) {
            if (arr[k] > arr[k + 1]) {
                int tmp = arr[k];
                arr[k] = arr[k + 1];
                arr[k + 1] = tmp;
            }
        }
    }
}

int *init_vector(Matrix *mtr) {
    int *arr = (int*)malloc(sizeof(int) * (mtr->cnt));
    for (int i = 0; i < mtr->cnt; i++) {
        int cnt_diff = 0;
        int *numb = (int*)malloc(sizeof(int) * mtr->data[i].cnt);
        for (int j = 0; j < mtr->data[i].cnt; j++) {
            numb[j] = mtr->data[i].data[j];
        }
        bubbleSort(numb, mtr->data[i]);
        for (int j = 0; j < mtr->data[i].cnt - 1; j++) {
            if (numb[j] != numb[j + 1]) {
                cnt_diff++;
            }
        }
        if (mtr->data[i].cnt != 0) {
        	cnt_diff++;
        }
        //cnt_diff++;
        arr[i] = cnt_diff;
        if (numb != NULL) {
        	free(numb);
        }
    }
    return arr;
}

void print_matrix(Matrix *mtr) {
    printf("Input matrix:\n");
    for (int i = 0; i < mtr->cnt; i++) {
        for (int j = 0; j < mtr->data[i].cnt; j++) {
            printf("%d ", mtr->data[i].data[j]);
        }
        printf("\n");
    }
}

void free_matrix(int *arr, Matrix *mtr) {
	if (arr != NULL) {
		free(arr);
	}
	for (int i = 0; i < mtr->cnt; i++) {
		if (mtr->data[i].data != NULL) {
			free(mtr->data[i].data);
		}
	}
	if (mtr->data != NULL) {
		free(mtr->data);
	}
	free(mtr);
}

Matrix *func1(Matrix *matr)
{
	if (matr == NULL) 
	{
		return NULL;
	}
	/*if (matr->data == NULL)
	{
		return NULL;
	}*/
	int max_size = matr->data[0].cnt;
	for (int i = 0; i < matr->cnt; ++i) {
		if (matr->data[i].cnt > max_size)
		{
			max_size = matr->data[i].cnt;
		}
	}
	Matrix *res = (Matrix*)malloc(sizeof(Matrix));
	res->cnt = matr->cnt;
	res->data = (line*)malloc(sizeof(line) * res->cnt);
	for (int i = 0; i < res->cnt; ++i) {
		res->data[i].cnt = max_size;
		res->data[i].data = (int*)malloc(sizeof(int) * res->data[i].cnt);
	}
	for (int i = 0; i < res->cnt; ++i) {
		for (int j = 0; j < max_size; ++j) {
			if (j < matr->data[i].cnt) {
				res->data[i].data[j] = matr->data[i].data[j];
			} else {
				res->data[i].data[j] = 0;
			}
		}
	}
	return res;
}

Matrix *func2(Matrix *matr)
{
	if (matr == NULL)
	{
		return NULL;
	}
	/*if(matr->data == NULL)
	{
		return NULL;
	}*/
	Matrix *res = (Matrix*)malloc(sizeof(Matrix));
	res->cnt = matr->cnt;
	res->data = (line*)malloc(sizeof(line) * res->cnt);
	for (int i = 0; i < res->cnt; ++i) {
		res->data[i].cnt = matr->data[i].cnt;
		res->data[i].data = (int*)malloc(sizeof(int) * res->data[i].cnt);
	}
	for (int i = 0; i < res->cnt; ++i) {
		for (int j = 0; j < res->data[i].cnt; ++j) {
			res->data[i].data[j] = matr->data[i].data[j];
		}
	}
	for (int i = 0; i < res->cnt; ++i) {
		if (res->data[i].cnt > 1)
		{
			int max_val = res->data[i].data[res->data[i].cnt - 1];
			int min_val = res->data[i].data[0], i_min = 0, i_max = res->data[i].cnt - 1;
			for (int j = 0; j < res->data[i].cnt; ++j) {
				if (res->data[i].data[j] < min_val)
				{
					min_val = res->data[i].data[j];
					i_min = j;
				}
			}
			for (int j = res->data[i].cnt - 1; j >= 0; j--) {
				if (res->data[i].data[j] > max_val)
				{
					max_val = res->data[i].data[j];
					i_max = j;
				}
			}
			int tmp = res->data[i].data[i_min];
			res->data[i].data[i_min] = res->data[i].data[i_max];
			res->data[i].data[i_max] = tmp;
		}
	}
	return res;
}

Matrix *func3(Matrix *matr)
{
	Matrix *res = (Matrix*)malloc(sizeof(Matrix));
	if (matr == NULL)
	{
		return NULL;
	}
	if (matr->cnt == 0)
	{
		res->cnt = 0;
		return res;
	}
	int sum = 0, i_beg = 0;
	for (int i = 0; i < matr->cnt; ++i) {
		if (matr->data[i].cnt == 0) {
			sum++;
		} else {
			i_beg = i;
			break;
		}
	}
	res->cnt = matr->cnt;
	res->data = (line*)malloc(sizeof(line) * res->cnt);	
	if (sum == matr->cnt)
	{
		for (int i = 0; i < res->cnt; ++i) {
			res->data[i].cnt = 0;
		}
		return res;
	}
	int max = matr->data[i_beg].data[0];
	for (int i = 0; i < matr->cnt; ++i) {
		for (int j = 0; j < matr->data[i].cnt; ++j) {
			if (matr->data[i].data[j] > max)
			{
				max = matr->data[i].data[j];
				i_beg = i;
			}
		}
	}
	res->data[0].cnt = matr->data[i_beg].cnt;
	res->data[0].data = (int*)malloc(sizeof(int) * res->data[0].cnt);
	for (int j = 0; j < res->data[0].cnt; ++j) {
		res->data[0].data[j] = matr->data[i_beg].data[j];
	}
	res->data[i_beg].cnt = matr->data[0].cnt;
	res->data[i_beg].data = (int*)malloc(sizeof(int) * res->data[i_beg].cnt);
	for (int j = 0; j < res->data[i_beg].cnt; ++j) {
		res->data[i_beg].data[j] = matr->data[0].data[j];
	}
	for (int i = 1; i < res->cnt; ++i) {
		if (i != i_beg)
		{
			res->data[i].cnt = matr->data[i].cnt;
			res->data[i].data = (int*)malloc(sizeof(int) * res->data[i].cnt);
			for (int j = 0; j < res->data[i].cnt; ++j) {
				res->data[i].data[j] = matr->data[i].data[j];
			}
		}
	}
	return res;
}
