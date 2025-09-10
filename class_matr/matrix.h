#ifndef CLASS_MATRIX
#define CLASS_MATRIX

class Matrix
{
    private:
        // number of rows
        int m;

        // number of columns
        int n;

        // m-by-n matrix
        double *matr;

        // epsilon
        const double EPS = 1.0e-12;
    
    public:
        // конструктор по умолчанию заполняющий матрицу из файла
        Matrix(const char *name_of_file);

        // конструктор по умолчанию заполняющий матрицу нулями
        Matrix(int m_rows, int n_columns);

        // конструктор по умолчанию базовый на всякий случай
        Matrix();

        // конструктор копий
        Matrix(const Matrix &Init_Matr);

        // Деструктор
        ~Matrix();


        // operator =
        Matrix& operator =(const Matrix &Use_Matr);
        /*
        возможно множественное присваивание  
        */     
        
        // operator +
        Matrix operator +(const Matrix &Use_Matr) const;

        // operator * matrix multiplication
        Matrix operator *(const Matrix &Use_Matr) const;

        // operator * multiplication by a number
        Matrix operator *(const double &number) const;

        // operator -
        Matrix operator -(const Matrix &Use_Matr) const;


        // print matrix to concole
        void print_matr() const;

        // swap two rows
        // нумерация строк начинается с 0
        void swap_rows(int l1, int l2, int start_col = 0);

        // умножение строки на число начиная с k-го элемента строки
        // нумерация элементов в строке начинается с 0
        void mult_row(double number, int l, int k = 0);

        // finding abs_max elem in k column starting from top_row
        int find_abs_max_in_column(int k, int top_row = 0);

        // substraction rows l2 - l1
        void sub_rows(int l1, int l2, int start_col = 0);

        // check double null
        bool is_null(double d) const;

        // Gaussian method
        // return number of swap rows
        int gauss_method();

        // epsilon clean matrix
        void eps_clean();

        // the rank of the matrix
        int rank() const;

        //read matrix from file
        void read_from_file(const char *name_of_file);

        // the determination of the matrix
        double det() const;

        // main step view of the matrix
        void main_step_view();

        // add identity matrix
        // приписывает к квадратной матрице справа единичную матрицу
        void add_ematr();

        // inverse matrix
        void inv_matr();

        // norm one
        // максимальная столбцовая норма
        double norm_one() const;

        // norm inf
        // максимальная строковая норма
        double norm_inf() const;
};

#endif // CLASS_MATRIX
