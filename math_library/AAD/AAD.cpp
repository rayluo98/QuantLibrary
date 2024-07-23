#include "AAD.h"
#include <algorithm>

//  Statics

size_t Node::numAdj = 1;
bool Tape::multi = false;

Tape globalTape;
thread_local Tape* Variable::tape = &globalTape;
