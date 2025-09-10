#include <stdio.h>
#include "matrix.h"

int main()
{
    Matrix A("data_matr01.txt");
    A.print_matr();

    /*
    A.gauss_method();
    A.print_matr();

    A.read_from_file("data_matr01.txt");
    printf("rank = %d\n", A.rank());
    printf("det  = %lf\n", A.det());

    A.read_from_file("data_matr01.txt");
    A.main_step_view();
    A.print_matr();
    */

    Matrix B(A);
    B.inv_matr();
    B.print_matr();
    (A*B).print_matr();

    printf("norm 1 of A = %lf\n", A.norm_one());
    printf("norm inf of A = %lf\n", A.norm_inf());

    return 0;
}