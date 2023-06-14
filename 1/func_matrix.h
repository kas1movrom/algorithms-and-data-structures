#ifndef FUNC_MATRIX_H_INCLUDED
#define FUNC_MATRIX_H_INCLUDED

typedef struct line line;
typedef struct Matrix Matrix;
Matrix *init_matrix(int cnt_matrix);
void bubbleSort(int *arr, line object);
int *init_vector(Matrix *mtr);
void print_matrix(Matrix *mtr);
void free_matrix(int *arr, Matrix *mtr);
Matrix *func1(Matrix *matr);
Matrix *func2(Matrix *matr);
Matrix *func3(Matrix *matr);

#endif
