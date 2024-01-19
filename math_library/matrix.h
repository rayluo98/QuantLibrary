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

    std::pair<int, int> get_dim() {
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

    matrix* mmult(matrix a, matrix b) {
        auto a_dims = a.get_dim();
        auto b_dims = b.get_dim();
        if (a_dims.second != b_dims.first)
            return new matrix(0,0); //need to replace with error handling
        matrix* res = new matrix(a_dims.first, b_dims.second);
        for (int i = 0; i < a_dims.first; ++i) {
            for (int j = 0; j < b_dims.second; ++j) {

                res->p[i][j] = dot(a.getRow(i), b.getCol(j));
            }
        }
        return res;
    }

    double* getRow(int i) {
        return p[i];
    }

    double* getCol(int i) {
        auto col = new double[row_num];
        for (int j = 0; j < row_num; j++) {
            col[j] = p[i][j];
        }
        return col;
    }

    double dot(double* v_1, double* v_2) {
        size_t length = sizeof(v_1) / sizeof(v_1[0]);
        double res = 0.0;
        for (size_t i = 0; i < length; ++i) {
            res += v_1[i] * v_2[i];
        }
        return res;
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

