#include <stdio.h>
#include <stdlib.h>

#include "func_line.h"
#include "func_matrix.h"

int main() {
    int cnt_matrix;
    printf("Enter a matrtix length:\n");
    check_positive(&cnt_matrix);
    Matrix *mtr = init_matrix(cnt_matrix);
    init_lines(mtr);
    /*int *arr = init_vector(mtr);
    printf("\n");
    print_vector(arr, mtr);*/
    printf("\n\n");
    print_matrix(mtr);
    printf("\n");

	Matrix *matr = func3(mtr);
	

	print_matrix(matr);
    
    //free_matrix(arr, mtr);
    return 0;
}
