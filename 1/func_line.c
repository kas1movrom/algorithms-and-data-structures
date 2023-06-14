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

void check_positive(int *value) {
    while ((scanf("%d", value) != 1) || (*value < 0)) {
        scanf("%*c");
        printf("Enter a positive integer!\n");
    }
}

void check_int(int *value) {
    while (scanf("%d", value) != 1) {
        scanf("%*c");
        printf("Enter a integer!\n");
    }
}

void init_lines(Matrix *mtr) {
    int cnt_line;
    printf("Enter matrix->cnt numbers - the length of each vector:\n");
    for (int i = 0; i < mtr->cnt; i++) {
        check_positive(&cnt_line);
        mtr->data[i].cnt = cnt_line;
        mtr->data[i].data = (int*)malloc(sizeof(int) * (mtr->data[i].cnt));
    }
    int value;
    printf("Enter each element of the matrix vector:\n");
    for (int i = 0; i < mtr->cnt; i++) {
        for (int j = 0; j < mtr->data[i].cnt; j++) {
            check_int(&value);
            mtr->data[i].data[j] = value;
        }
    }
}

void print_vector(int *arr, Matrix *mtr) {
    printf("Output vector, each element is the number of different elements in the rox of the matrix with the same index:\n");
    for (int i = 0; i < mtr->cnt; i++) {
        printf("%d ", arr[i]);
    }
}


