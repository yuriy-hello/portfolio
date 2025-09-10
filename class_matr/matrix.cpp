#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "matrix.h"

// конструктор по умолчанию заполняющий матрицу из файла
Matrix:: Matrix(const char *name_of_file)
{
    FILE* input_file_ptr = NULL;
    input_file_ptr = fopen(name_of_file, "r");

    if(input_file_ptr == NULL)
    {
        printf("file %s doesn't found\n", name_of_file);
        exit(-1);
    }

    if(fscanf(input_file_ptr, "%d", &m) != 1)
    {
        printf("can't read number of rows\n");
        exit(-1);
    }

    if(fscanf(input_file_ptr, "%d", &n) != 1)
    {
        printf("can't read number of columns\n");
        exit(-1);
    }

    if((m < 1) || (n < 1))
    {
        printf("the size of matrix is set incorrectly\n");
        exit(-1);
    }

    matr = new double[m * n];
    double number = 0;

    for(int i = 0; i < m * n; ++i)
    {
        if(fscanf(input_file_ptr, "%lf", &number) != 1)
        {
            printf("see data_matr file\n row : %d\n col : %d\n", i / n + 1, i % n + 1);
            exit(-1);
        }
        matr[i] = number;
    }

    fclose(input_file_ptr);
}

// конструктор по умолчанию заполняющий матрицу нулями
Matrix:: Matrix(int m_rows, int n_columns):
    m(m_rows),
    n(n_columns),
    matr(new double[m * n])
{
    for(int i = 0; i < m * n; ++i) matr[i] = 0;
}

// конструктор по умолчанию базовый на всякий случай
Matrix:: Matrix():
    m(0),
    n(0),
    matr(NULL)
    {}

// конструктор копий
Matrix:: Matrix(const Matrix &Init_Matr)
{
    this -> m = Init_Matr.m;
    this -> n = Init_Matr.n;
    this -> matr = new double[m * n];

    for(int i = 0; i < m * n; ++i)
        (this -> matr)[i] = Init_Matr.matr[i];
        /*
        можно без скобок this -> matr[i]
        так как приоритет у операцмм -> выше чем у []
        */
}

// Деструктор
Matrix:: ~Matrix()
{
    delete[] matr;
}

// operator =
Matrix& Matrix:: operator =(const Matrix &Use_Matr)
{
    this -> m = Use_Matr.m;
    this -> n = Use_Matr.n;
    delete[] this -> matr;
    this -> matr = new double[m * n];

    for(int i = 0; i < m * n; ++i)
    {
        matr[i] = Use_Matr.matr[i];
    }

    return *this; // this it is pointer of object (*this).m is equal to this -> m
}

// operator +
Matrix Matrix:: operator +(const Matrix &Use_Matr) const
{
    if((m != Use_Matr.m) || (n != Use_Matr.n))
    {
        printf("operator + : sum matrix have different dimensions\n");
        exit(-1);
    }

    Matrix Sum_Matr(*this);
    for(int i = 0; i < m * n; ++i)
    {
        Sum_Matr.matr[i] += Use_Matr.matr[i];
    }

    return Sum_Matr;
}

// operator * matrix multiplication
Matrix Matrix:: operator *(const Matrix &Use_Matr) const
{
    if(n != Use_Matr.m)
    {
        printf("operator * matrix multiplication: no correct matrix dimension\n");
        exit(-1);
    }

    Matrix Pr_Matr(m, Use_Matr.n);

    for(int i_row = 0; i_row < m; ++i_row)
    {
        for(int j_col = 0; j_col < Use_Matr.n; ++j_col)
        {
            double sum = 0;
            for(int i = 0; i < n; ++i)
            {
                sum += matr[i_row * n + i] * Use_Matr.matr[i * Use_Matr.n + j_col];
            }
            Pr_Matr.matr[i_row * Use_Matr.n + j_col] = sum;
        }
    }

    return Pr_Matr;
}

// operator * multiplication by a number
Matrix Matrix:: operator *(const double &number) const
{
    Matrix Pr_Matr(*this);
    for(int i = 0; i < m * n; ++i)
    {
        Pr_Matr.matr[i] *= number;
    }

    return Pr_Matr;
}

// operator -
Matrix Matrix:: operator -(const Matrix &Use_Matr) const
{
    if((m != Use_Matr.m) || (n != Use_Matr.n))
    {
        printf("operator - : substraction matrix have different dimensions\n");
        exit(-1);
    }

    Matrix Sum_Matr(*this);
    for(int i = 0; i < m * n; ++i)
    {
        Sum_Matr.matr[i] -= Use_Matr.matr[i];
    }

    return Sum_Matr;
}

// print matrix to concole
void Matrix:: print_matr() const
{
    printf("\n");
    for(int i = 0; i < m; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
            printf("%10.6lf ", matr[i * n + j]);
        }
        printf("\n");
    }
    printf("\n");
}

// swap two rows
// нумерация строк начинается с 0
void Matrix:: swap_rows(int l1, int l2, int start_col)
{
    if(l1 == l2) return;

    double tmp_c = 0;
    for(int i = start_col; i < n; ++i)
    {
        tmp_c = matr[l1 * n + i];
        matr[l1 * n + i] = matr[l2 * n + i];
        matr[l2 * n + i] = tmp_c;
    }
}

// умножение строки на число начиная с k-го элемента строки
// нумерация элементов в строке начинается с 0
void Matrix:: mult_row(double number, int l, int k)
{
    for(int i = k; i < n; ++i)
    {
        matr[l * n + i] *= number;
    }
}

// finding abs_max elem in k column starting from top_row
int Matrix:: find_abs_max_in_column(int k, int top_row)
{
    double abs_max = fabs(matr[top_row * n + k]);
    int l = top_row;
    double tmp_c = 0;
    for(int i = top_row + 1; i < m; ++i)
    {
        tmp_c = fabs(matr[i * n + k]);
        if(tmp_c > abs_max)
        {
            abs_max = tmp_c;
            l = i;
        }
    }

    return l;
}

// check double null
bool Matrix:: is_null(double d) const
{
    if(fabs(d) <= EPS ) return true;
    else return false;
}

// substraction rows l2 - l1
void Matrix:: sub_rows(int l1, int l2, int start_col)
{
    double k = matr[l2 * n + start_col] / matr[l1 * n + start_col];
    matr[l2 * n + start_col] = 0;

    for(int i = start_col + 1; i < n; ++i)
    {

        matr[l2 * n + i] -= k * matr[l1 * n + i];
    }
}

// Gaussian method
// return number of swap rows
int Matrix:: gauss_method()
{
    int num_swap = 0;
    for(int i = 0, j = 0; (i < m - 1) && (j < n); )
    {
        int abs_max_i = find_abs_max_in_column(j, i);

        if(is_null(matr[abs_max_i * n + j]))
        {
            ++j;
            continue;
        }

        swap_rows(i, abs_max_i, j);
        ++num_swap;

        for(int null_row = i + 1; null_row < m; ++null_row)
        {
            sub_rows(i, null_row, j);
        }

        ++i;
        ++j;
        //print_matr();
    }

    return num_swap;
}

// epsilon clean matrix
void Matrix:: eps_clean()
{
    for(int i = 0; i < m * n; ++i)
    {
        if(is_null(matr[i])) matr[i] = 0.0;
    }
}

// rank of matrix
int Matrix:: rank() const
{
    Matrix Use_Matr(*this);
    int r = 0;

    Use_Matr.gauss_method();
    Use_Matr.eps_clean();

    for(int i = 0, j = 0; (i < m) && (j < n); )
    {
        if(is_null(Use_Matr.matr[i * n + j]))
        {
            ++j;
            continue;
        }

        ++i;
        ++j;
        ++r;

    }

    return r;
}

//read matrix from file
void Matrix:: read_from_file(const char *name_of_file)
{
    FILE* input_file_ptr = NULL;
    input_file_ptr = fopen(name_of_file, "r");

    if(input_file_ptr == NULL)
    {
        printf("file %s doesn't found\n", name_of_file);
        exit(-1);
    }

    if(fscanf(input_file_ptr, "%d", &m) != 1)
    {
        printf("can't read number of rows\n");
        exit(-1);
    }

    if(fscanf(input_file_ptr, "%d", &n) != 1)
    {
        printf("can't read number of columns\n");
        exit(-1);
    }

    if((m < 1) || (n < 1))
    {
        printf("the size of matrix is set incorrectly\n");
        exit(-1);
    }

    matr = new double[m * n];
    double number = 0.0;

    for(int i = 0; i < m * n; ++i)
    {
        if(fscanf(input_file_ptr, "%lf", &number) != 1)
        {
            printf("see data_matr file\n row : %d\n col : %d\n", i / n + 1, i % n + 1);
            exit(-1);
        }
        matr[i] = number;
    }

    fclose(input_file_ptr);
}

// the determination of the matrix
double Matrix:: det() const
{
    if(m != n)
    {
        printf("matrix isn't square, det is undefined\n");
        exit(-1);
    }

    double d = 1.0;
    Matrix Use_Matr(*this);
    int num_swap = Use_Matr.gauss_method();
    Use_Matr.eps_clean();

    for(int i = 0; i < m; ++i)
    {
        d *= Use_Matr.matr[i * (n + 1)];
    }

    if(num_swap % 2 == 1) d *= -1.0;

    return d;
}

// main step view of the matrix
void Matrix:: main_step_view()
{
    gauss_method();
    eps_clean();

    for(int i = 0, j = 0; (i < m) && (j < n); )
    {
        if(is_null(matr[i * n + j]))
        {
            ++j;
            continue;
        }

        for(int nul_row = i - 1; nul_row >= 0; --nul_row)
        {
            sub_rows(i, nul_row, j);
        }

        ++i;
        ++j;
        //print_matr();
    }

    for(int i = 0, j = 0; (i < m) && (j < n); )
    {
        if(is_null(matr[i * n + j]))
        {
            ++j;
            continue;
        }
        double k = 1.0 / matr[i * n + j];
        matr[i * n + j] = 1.0;

        mult_row(k, i, j +1);
        ++i;
        ++j;
    }
}

// add identity matrix
// приписывает к квадратной матрице справа единичную матрицу
void Matrix:: add_ematr()
{
    Matrix Use_Matr(*this);
    delete[] matr;
    int old_n = n;
    n *= 2;
    matr = new double[m * n];

    for(int i = 0; i < m; ++i)
    {
        for(int j = 0; j < old_n; ++j)
        {
            matr[i * n + j] = Use_Matr.matr[i * old_n + j];
        }
        for(int j = old_n; j < n; ++j)
        {
            if(j - old_n == i) matr[i * n + j] = 1.0;
            else matr[i * n + j] = 0.0;
        }
    }
}

// inverse matrix
void Matrix:: inv_matr()
{
    int r = rank();
    if((r != m) || (m != n))
    {
        printf("inverse matrix isn't exist, sach as rank != m or m != n\n");
        exit(-1);
    }

    Matrix Use_Matr(*this);
    Use_Matr.add_ematr();
    Use_Matr.main_step_view();

    for(int i = 0; i < m; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
            matr[i * n + j] = Use_Matr.matr[i * 2 * n + j + n];
        }
    }
}