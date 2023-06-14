#ifndef FUNC_LINE_H_INCLUDED
#define FUNC_LINE_H_INCLUDED

typedef struct line line;
typedef struct Matrix Matrix;
void check_positive(int *value);
void check_int(int *value);
void init_lines(Matrix *mtr);
void print_vector(int *arr, Matrix *mtr);

#endif
