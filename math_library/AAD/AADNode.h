#pragma once

/*
Written by Antoine Savine in 2018

This code is the strict IP of Antoine Savine

License to use and alter this code for personal and commercial applications
is freely granted to any person or company who purchased a copy of the book

Modern Computational Finance: AAD and Parallel Simulations
Antoine Savine
Wiley, 2018

As long as this comment is preserved at the top of the file
*/

//  Implementation of Node = record on tape

#include <exception>
using namespace std;

class Node 
{
	friend class Tape;
	friend class Variable;
	friend auto setNumResultsForAAD(const bool, const size_t);
	friend struct numResultsResetterForAAD;

    //  The adjoint(s) 
	//	in single case, self held 
	double			mAdjoint = 0;
	//	in multi case, held separately and accessed by pointer 
    double*         pAdjoints;  

	//  Data lives in separate memory

    //  the n derivatives to arguments,
    double*         pDerivatives;    

    //  the n pointers to the adjoints of arguments
    double**        pAdjPtrs;

    //  Variable of adjoints (results) to propagate, usually 1
    //  See chapter 14
    static size_t   numAdj;

    //  Variable of childs (arguments)
    const size_t    n;

public:

    Node(const size_t N = 0) : n(N) {}

    //  Access to adjoint(s)
	//	single
    double& adjoint() 
    {
		return mAdjoint;
	}
	//	multi
	double& adjoint(const size_t n) { return pAdjoints[n]; }
    
    //  Back-propagate adjoints to arguments adjoints

    void propagateOne() 
{
		//  Nothing to propagate
		if (!n || !mAdjoint) return;

		for (size_t i = 0; i < n; ++i)
        {
			*(pAdjPtrs[i]) += pDerivatives[i] * mAdjoint;
        }
    }

    void propagateAll()
{
        //  No adjoint to propagate
        if (!n || all_of(pAdjoints, pAdjoints + numAdj,
            [](const double& x) { return !x; }))
            return;

        for (size_t i = 0; i < n; ++i)
        {
            double *adjPtrs = pAdjPtrs[i], ders = pDerivatives[i];

            //  Vectorized!
            for (size_t j = 0; j < numAdj; ++j)
            {
                adjPtrs[j] += ders * pAdjoints[j];
            }
        }
    }
};