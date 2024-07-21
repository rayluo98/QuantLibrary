#pragma once

#include "iostream"
#include <vector>
using namespace std;

namespace MatLib
{
    class Matrix
    {
    private:
        size_t row_num, col_num;
        double **p;
        void initialize()
        {
            p = new double *[row_num];
            for (size_t i = 0; i < row_num; i++)
            {
                p[i] = new double[col_num];
            }
        }

    public:
        Matrix();
        Matrix(Matrix& mat);
        Matrix(int rows, int cols);
        Matrix(double **_p, int rows, int cols);
        Matrix(vector<vector<double>> p);
        ~Matrix();
        std::pair<int, int> get_dim();
        Matrix &operator=(double *a);
        Matrix &operator=(const vector<double> &a);
        Matrix &operator+=(const Matrix &m);
        Matrix &operator+(const Matrix &m);
        Matrix &operator*=(double a);
        Matrix &operator*(double a);
        double operator()(int a, int b);
        Matrix *mmult(Matrix a, Matrix b);
        double *getRow(int i);
        double *getCol(int i);
        double dot(double *v_1, double *v_2);
        double sum();
        double sum(int n);
        void show() const;
    };

    Matrix identity(const int numRows, const int numCols, double val = 1, int rowStart = 0) {
        // initialize 2d array first then convert
        vector<vector<double>> id; // [numRows] [numCols] ;
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                if (i >= rowStart and i == j) {
                    id[i][j] = val;
                }
            }
        }
        Matrix mat(id);
        return mat; //todo
    }

}
