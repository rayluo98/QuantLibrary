#pragma once

#include "iostream"
#include <vector>
using namespace std;

namespace MatLib
{
    class Matrix
    {
    private:
        int row_num, col_num;
        double **p;
        void initialize()
        {
            p = new double *[row_num];
            for (int i = 0; i < row_num; i++)
            {
                p[i] = new double[col_num];
            }
        }

    public:
        Matrix(int rows, int cols);
        Matrix(double **_p, int rows, int cols);
        ~Matrix();
        std::pair<int, int> get_dim();
        Matrix &operator=(double *a);
        Matrix &operator=(const vector<double> &a);
        Matrix &operator+=(const Matrix &m);
        Matrix &operator+(const Matrix &m);
        Matrix &operator*=(double a);
        Matrix &operator*(double a);
        Matrix *mmult(Matrix a, Matrix b);
        double *getRow(int i);
        double *getCol(int i);
        double dot(double *v_1, double *v_2);
        double sum();
        double sum(int n);
        void show() const;
    };
}
