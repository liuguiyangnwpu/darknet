#ifndef MATRIX_H
#define MATRIX_H
#include "darknet.h"

matrix copy_matrix(matrix m);
void print_matrix(matrix m);

matrix hold_out_matrix(matrix *m, int n);
matrix resize_matrix(matrix m, int size);

float *pop_column(matrix *m, int c);
void matrix_to_csv(matrix m);
float matrix_topk_accuracy(matrix truth, matrix guess, int k);
void matrix_add_matrix(matrix from, matrix to);
void scale_matrix(matrix m, float scale);
matrix csv_to_matrix(char *filename);
matrix make_matrix(int rows, int cols);
void free_matrix(matrix m);

#endif
