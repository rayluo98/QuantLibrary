#pragma once

#include <memory>

//  So we can instrument Gaussians like standard math functions
#include "math_library/gaussians.h"

#define AADET   true

#if AADET

#include "AADExpr.h"

#else

#include "AADNumber.h"

#endif

//	RAII: reset dimension 1 on destruction
struct numResultsResetterForAAD
{
	~numResultsResetterForAAD()
	{
		Tape::multi = false;
		Node::numAdj = 1;
	}
};

//  Routine: set dimension and get RAII resetter
inline auto setNumResultsForAAD(const bool multi = false, const size_t numResults = 1)
{
	Tape::multi = multi;
	Node::numAdj = numResults;
	return make_unique<numResultsResetterForAAD>();
}

//  Other utilities

//	Put collection on tape
template <class IT>
inline void putOnTape(IT begin, IT end)
{
    for_each(begin, end, [](Number& n) {n.putOnTape(); });
}

//	Convert collection between double and Number or reverse
template<class It1, class It2>
inline void convertCollection(It1 srcBegin, It1 srcEnd, It2 destBegin)
{
    using destType = remove_reference_t<decltype(*destBegin)>;
    transform(srcBegin, srcEnd, destBegin, 
        [](const auto& source) { return destType(source); });
}
