#include "iostream"
#include <vector>
using namespace std;

class matrix
{
private:
    int row_num, col_num;
    double** p;
    void initialize()
    {
        p = new double* [row_num];
        for (int i = 0; i < row_num; i++)
        {
            p[i] = new double[col_num];
        }
    }
public:
    matrix(int rows, int cols)
    {
        row_num = rows;
        col_num = cols;
        initialize();
        for (int i = 0; i < row_num; i++)
        {
            for (int j = 0; j < col_num; j++) {
                p[i][j] = 0;
            }
        }
    }
    ~matrix()
    {
        for (int i = 0; i < row_num; i++)
        {
            delete[] p[i];
        }
        delete[] p;
    }

    std::pair(int, int) get_dim() {
        return make_pair(row_num, col_num);
    }


    matrix& operator=(double* a)
    {
        for (int i = 0; i < row_num; i++)
        {
            for (int j = 0; j < col_num; j++)
                p[i][j] = *(a + i * col_num + j);
        }
        return *this;
    }
    matrix& operator=(const vector<double>& a)
    {
        for (int i = 0; i < row_num; i++)
        {
            for (int j = 0; j < col_num; j++)
                p[i][j] = a[i * col_num + j];
        }
        return *this;
    }


    matrix& operator+=(const matrix& m)
    {
        for (int i = 0; i < row_num; i++)
        {
            for (int j = 0; j < col_num; j++)
                p[i][j] += m.p[i][j];
        }
        return *this;
    }


    matrix& operator+(const matrix& m)
    {
        return (*this) += m;
    }


    matrix& operator*= (double a)
    {
        for (int i = 0; i < row_num; i++)
        {
            for (int j = 0; j < col_num; j++)
                p[i][j] = p[i][j] * a;
        }
        return *this;
    }

    matrix& operator*(double a)
    {
        return (*this) *= a;

    }

    matrix operation* (matrix a, matrix b) {
        auto a_dims = a.get_dim();
        auto b_dims = b.get_dim();
        if (a_dims.second != b_dims.first)
            return new matrix(0,0); //need to replace with error handling
        matrix res = new matrix(a_dims.first, b_dims.second);
        for (int i = 0; i < a_dims.first; ++i) {
            for (int j = 0; j < b_dims.second; ++j) {
                res[i][j] = 0; // to fill
            }
        }
    }
    
    double sum()
    {
        double ans = 0;
        for (int i = 0; i < row_num; i++)
        {
            for (int j = 0; j < col_num; j++)
                ans += p[i][j];
        }
        return ans;
    }
    double sum(int n)
    {
        double ans = 0;
        for (int i = 0; i < row_num; i++)
        {
            for (int j = 0; j < n; j++)
                ans += p[i][j];
        }
        return ans;
    }

    void show() const
    {
        for (int i = 0; i < row_num; i++)
        {
            for (int j = 0; j < col_num; j++)
                cout << p[i][j] << " ";
            cout << endl;

        }
        cout << endl;
    }
};

