#include <vector>
#include <iostream>
#include "../matrix.h"
#pragma once

using namespace std;

struct stdForm {
    vector<double> cost;
    vector<double> constraints;
    vector<double> threshold;
};

class Simplex
{

private:
    int rows, cols;
    // Stores coefficients of all the variables
    vector<vector<double>> tableau;
    vector<pair<double, double>> solution;
    double objectVal;

    float maximum;

    bool isUnbounded;

public:
    vector<vector<double>> identity(int numRows, int numCols, double val = 1, int rowStart = 0) {
        vector<vector<double>> res;
        for (int i = 0; i < numRows; ++i) {
            res.push_back(vector<double>());
            for (int j = 0; j < numCols; ++j) {
                res[i].push_back((i == j and i >= rowStart) ? val : 0);
            }
        }
        return res;
    }

    /*
    stdForm standardForm(vector<double> cost, vector<double> greaterThans = {}, vector<double> gtThreshold = {},
        vector<double> lessThans = {}, vector<double> ltThreshold = {}, vector<double> equalities = {},
        vector<double> eqThreshold = {}, bool maximization = true) {
        stdForm res;
        int newVars = 0;
        int numRows = 0;
        if (gtThreshold.size() != 0) {
            newVars += gtThreshold.size();
            numRows += gtThreshold.size();
        }
        if (ltThreshold.size() != 0) {
            newVars += ltThreshold.size();
            numRows += ltThreshold.size();
        }
        if (eqThreshold.size() != 0) {
            numRows += eqThreshold.size();
        }

        if (!maximization) {
            for (auto& i : cost) {
                i = -1 * i;
            }
        }

        if (newVars == 0) {
            res.cost = cost;
            res.constraints = equalities;
            res.threshold = eqThreshold;
            return res;
        }

        vector<double> newCost;
        newCost.reserve(cost.size() + newVars);
        newCost.insert(cost.begin(), cost.end(), cost.end());
        vector<double> newVarZeros = vector<double>(newVars, 0);
        newCost.insert(newVarZeros.begin(), newVarZeros.end(), newVarZeros.end());

        vector<double> constraints = {};
        vector<double> threshold = {};

        vector<double> oldConstraints = [(greaterThans, gtThreshold, -1), (lessThans, ltThreshold, 1),
            (equalities, eqThreshold, 0)];

        double offset = 0;

        for (constraintList, oldThreshold, coefficient) in oldConstraints{
            constraints += [c + r for c, r in zip(constraintList,
                identity(numRows, newVars, coefficient, offset))]
        }
        threshold.push_back(oldThreshold);
        offset += oldThreshold.size();

        res.cost = newCost;
        res.constraints = constraints;
        res.threshold = threshold;
        return res;
    }
    */

    double dot(vector<double> a, vector<double> b) {
        double sum = 0;
        if (a.size() != b.size()) cout << "Sizes are not equal" << endl;
        for (int i = 0; i < a.size(); ++i) sum += a[i] * b[i];
        return sum;
    }

    vector<double> column(vector<vector<double>> A, int j) {
        vector<double> x;
        for (int i = 0; i < A.size(); i++) {
            x.push_back(A[i][j]);
        }
        return x;
    }

    vector<vector<double>> transpose(vector<vector<double>> A) {
        if (A.size() == 0) return A;
        vector<vector<double>> A_t;
        for (int i = 0; i < A[0].size(); i++) {
            A_t.push_back(column(A, i));
        }
        return A_t;
    }

    bool isPivot(vector<double> col) {
        vector<double> zero_col;
        double sum = 0;
        for (auto i : col) {
            if (i == 0) zero_col.push_back(i);
            sum += i;
        }
        return (sum == 1) && (col.size() - 1 == zero_col.size());
    }

    double variableValueForPivotColumn(vector<vector<double>> tableau, vector<double> column) {
        vector<double> pivotRow;
        int i = 0;
        for (;i < column.size(); ++i)
            if (column[i] == 1) break;
        return tableau[i][-1];
    }

    // assume the last m columns of A are the slack variables; the initial basis is 
    // the set of slack variables
    vector<vector<double>> initialTableau(vector<double> c, vector<vector<double>> A, vector<double> b) {
        vector<vector<double>> tableau;
        for (int i = 0; i < A.size(); ++i) {
            vector<double> row;
            row = A[i];
            row.push_back(b[i]);
            tableau.push_back(row);
        }
        for (int i = 0; i < c.size(); ++i) {
            vector<double> row;
            row = c;
            row.push_back(0);
            tableau.push_back(row);
        }
        return tableau;
    }


    //the pivot columns denote which variables are used
    vector<pair<double, double>> primalSolution(vector<vector<double>> tableau) {
        auto column = transpose(tableau);
        vector<int> indices;
        for (int i = 0; i < column.size() - 1; ++i) {
            if (isPivot(column[i])) {
                indices.push_back(i);
            }
        }
        vector<pair<double, double>> result;
        for (auto colIndex : indices) {
            result.push_back(make_pair(colIndex, variableValueForPivotColumn(tableau, column[colIndex])));
        }
    }

    double objectiveValue(vector<vector<double>> tableau) {
        return -1 * tableau[tableau.size() - 1][tableau.size() - 1];
    }

    bool canImprove(vector<vector<double>> tableau) {
        auto lastRow = tableau[tableau.size() - 1];
        for (int i = 0; i < lastRow.size() - 1; ++i) {
            if (lastRow[i] > 0) {
                return true;
            }
        }
        return false;
    }

    bool moreThanOneMin(vector<pair<int, double>> L) {
        if (L.size() <= 1) {
            return false;
        }
        // x, y = heapq.nsmallest(2, L, key = lambda x : x[1])
        // return x == y
        return false;
    }

    //pick minimum positive index of the last row
    pair<int, int> findPivotIndex(vector<vector<double>> tableau) {
        vector<pair<int, double>> column_choices;
        size_t n = tableau.size();
        int col = 0;
        double min = DBL_MAX;
        for (size_t i = 0; i < tableau[n - 1].size() - 1; ++i) {
            if (tableau[n - 1][i] < min){
                min = tableau[n - 1][i];
                col = int(i);
            }
            //column_choices.push_back(make_pair(i, ));
        }
        size_t i = 0;
        // check if unbounded
        for (i = 0; i < tableau[col].size(); ++i) {
            if (tableau[col][i] > 0) break;
        }
        if (i == tableau[col].size() - 1) {
            throw invalid_argument("Linear program is unbounded.");
        }

        // check for degeneracy: more than one minimizer of the quotient
        vector<pair<int, double>> quotients;
        int row = 0;
        min = DBL_MAX;
        for (size_t j = 0; j < tableau.size() - 1; ++j) {
            if (tableau[i][col] > 0) {
                auto val = tableau[i][tableau[col].size() - 1] / tableau[i][col];
                quotients.push_back(make_pair(j, val));
                if (val < min) {
                    min = val;
                    row = int(j);
                }
            }
        }
        if (moreThanOneMin(quotients)) { throw invalid_argument("Linear program is degenerate."); }

        return make_pair(row, col);
    }

                
    void pivotAbout(vector<vector<double>>& tableau, pair<int, int> pivot) {
        int i = pivot.first;
        int j = pivot.second;

        double pivotDenom = tableau[i][j];
        for (int n = 0; n < tableau[i].size(); ++n) {
            tableau[i][n] /= pivotDenom;
        }
        for (int m = 0; m < tableau.size(); ++m) {
            if (m != i) {
                vector<double> pivotRowMultiple;
                for (int h = 0; h < tableau[i].size(); ++h) {
                    pivotRowMultiple.push_back(tableau[i][h] * tableau[m][j]);
                }
                for (int h = 0; h < tableau.size(); ++h) {
                    tableau[m][h] = tableau[m][h] - pivotRowMultiple[h];
                }
            }
        }
    }

    Simplex(vector<double> c, vector<vector<double>>  A, vector<double> b) {
        tableau = initialTableau(c, A, b);
        cout << "Initial tableau:" << endl;
        for (auto row : tableau) {
            while (canImprove(tableau)) {
                auto pivot = findPivotIndex(tableau);
                pivotAbout(tableau, pivot);
            }
        }
        solution = primalSolution(tableau);
        objectVal = objectiveValue(tableau);
    }

};

/*
int main()
{
    vector<double> c = { 300, 250, 450 };
    vector<vector<double>> A = { {15, 20, 25}, {35, 60, 60}, {20, 30, 25},{0, 250, 0} };
    vector<double> b = { 1200, 3000, 1500, 500 };

    //add slack variables by hand
    A[0].push_back(1); A[0].push_back(0); A[0].push_back(0); A[0].push_back(0);
    A[1].push_back(0); A[1].push_back(1); A[1].push_back(0); A[1].push_back(0);
    A[2].push_back(0); A[2].push_back(0); A[2].push_back(1); A[2].push_back(0);
    A[3].push_back(0); A[3].push_back(0); A[3].push_back(0); A[3].push_back(1);
    c.push_back(0); c.push_back(0); c.push_back(0); c.push_back(0);

    Simplex simplexSolver = Simplex(c, A, b);
}*/