#include "matrix.h"

using namespace std;

namespace MatLib
{
    Matrix::Matrix() {
        row_num = 0; col_num = 0;
        initialize();
    }

    Matrix::Matrix(int rows, int cols)
    {
        row_num = rows;
        col_num = cols;
        initialize();
        for (int i = 0; i < row_num; i++)
        {
            for (int j = 0; j < col_num; j++)
            {
                p[i][j] = 0;
            }
        }
    }

    Matrix::Matrix(vector<vector<double>> vec) {
        row_num = vec.size();
        if (row_num == 0) {
            col_num = 0;
        }
        else {
            col_num = vec[0].size();
        }
        initialize();
        for (int i = 0; i < row_num; i++)
        {
            for (int j = 0; j < col_num; j++)
            {
                p[i][j] = vec[i][j];
            }
        }
    }

    Matrix::Matrix(Matrix& mat){
        auto dims = mat.get_dim();
        row_num = dims.first;
        col_num = dims.second;
        initialize();
        for (int i = 0; i < row_num; i++)
        {
            for (int j = 0; j < col_num; j++)
            {
                p[i][j] = mat(i,j);
            }
        }
    }

    Matrix::Matrix(double **_p, int rows, int cols)
    {
        row_num = rows;
        col_num = cols;
        initialize();
        for (int i = 0; i < row_num; i++)
        {
            for (int j = 0; j < col_num; j++)
            {
                p[i][j] = _p[i][j];
            }
        }
    }

    Matrix::~Matrix()
    {
        for (int i = 0; i < row_num; i++)
        {
            delete[] p[i];
        }
        delete[] p;
    }

    std::pair<int, int> Matrix::get_dim()
    {
        return make_pair(row_num, col_num);
    }

    double Matrix::operator()(int a, int b){
        return p[a][b];
    }

    Matrix &Matrix::operator=(double *a)
    {
        for (int i = 0; i < row_num; i++)
        {
            for (int j = 0; j < col_num; j++)
                p[i][j] = *(a + i * col_num + j);
        }
        return *this;
    }
    Matrix &Matrix::operator=(const vector<double> &a)
    {
        for (int i = 0; i < row_num; i++)
        {
            for (int j = 0; j < col_num; j++)
                p[i][j] = a[i * col_num + j];
        }
        return *this;
    }

    Matrix &Matrix::operator+=(const Matrix &m)
    {
        for (int i = 0; i < row_num; i++)
        {
            for (int j = 0; j < col_num; j++)
                p[i][j] += m.p[i][j];
        }
        return *this;
    }

    Matrix &Matrix::operator+(const Matrix &m)
    {
        return (*this) += m;
    }

    Matrix &Matrix::operator*=(double a)
    {
        for (int i = 0; i < row_num; i++)
        {
            for (int j = 0; j < col_num; j++)
                p[i][j] = p[i][j] * a;
        }
        return *this;
    }

    Matrix &Matrix::operator*(double a)
    {
        return (*this) *= a;
    }

    Matrix *Matrix::mmult(Matrix a, Matrix b)
    {
        auto a_dims = a.get_dim();
        auto b_dims = b.get_dim();
        if (a_dims.second != b_dims.first)
            return new Matrix(0, 0); // need to replace with error handling
        Matrix *res = new Matrix(a_dims.first, b_dims.second);
        for (int i = 0; i < a_dims.first; ++i)
        {
            for (int j = 0; j < b_dims.second; ++j)
            {

                res->p[i][j] = dot(a.getRow(i), b.getCol(j));
            }
        }
        return res;
    }

    double *Matrix::getRow(int i)
    {
        return p[i];
    }

    double *Matrix::getCol(int i)
    {
        auto col = new double[row_num];
        for (int j = 0; j < row_num; j++)
        {
            col[j] = p[i][j];
        }
        return col;
    }

    double Matrix::dot(double *v_1, double *v_2)
    {
        size_t length = *(&v_1 + 1) - v_1;
        double res = 0.0;
        for (int i = 0; i < length; ++i)
        {
            res += v_1[i] * v_2[i];
        }
        return res;
    }

    double Matrix::sum()
    {
        double ans = 0;
        for (size_t i = 0; i < row_num; i++)
        {
            for (size_t j = 0; j < col_num; j++)
                ans += p[i][j];
        }
        return ans;
    }
    double Matrix::sum(int n)
    {
        double ans = 0;
        for (int i = 0; i < row_num; i++)
        {
            for (int j = 0; j < n; j++)
                ans += p[i][j];
        }
        return ans;
    }

    void Matrix::show() const
    {
        for (int i = 0; i < row_num; i++)
        {
            for (int j = 0; j < col_num; j++)
                cout << p[i][j] << " ";
            cout << endl;
        }
        cout << endl;
    }
    /// Method to calculate the Determinant of a Matrix

    // NOT SURE WHAT THESE ARE DOING SINCE MATRIX ISN'T DECLARED AS A TEMPLATE CLASS
    // template <typename T>
    // public double Matrix::Det(Matrix<T> mat, int n)
    // {
    //     if (mat.row_length() != mat.col_length())
    //         assert(("The Matrix must be a square matrix", false));

    //     double D = 0;
    //     if (n == 1)
    //         return mat.data_mat[0][0];

    //     int sign = 1;
    //     Matrix<T> temp = zeros<T>(n, n);

    //     for (int f = 0; f < n; f++)
    //     {
    //         temp = cofactor(mat, temp, 0, f);
    //         D += sign * mat.data_mat[0][f] * determinant(temp, n - 1);
    //         sign = -sign;
    //     }

    //     return D;
    // }

    // /// Helper method to calculate cofactor
    // template <typename T>
    // Matrix<T> Matrix::cofactor(Matrix<T> mat, Matrix<T> temp, int p, int q)
    // {
    //     int i = 0, j = 0;
    //     for (int row = 0; row < mat.row_length(); row++)
    //     {
    //         for (int col = 0; col < mat.col_length(); col++)
    //         {
    //             if (row != p && col != q)
    //             {
    //                 temp.data_mat[i][j++] = mat.data_mat[row][col];
    //                 if (j == mat.col_length() - 1)
    //                 {
    //                     j = 0;
    //                     i++;
    //                 }
    //             }
    //         }
    //     }

    //     return temp;
    // }
}
