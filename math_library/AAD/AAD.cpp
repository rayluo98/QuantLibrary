
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

#include "AAD.h"
#include <algorithm>

//  Statics

size_t Node::numAdj = 1;
bool Tape::multi = false;

Tape globalTape;
thread_local Tape* Number::tape = &globalTape;
