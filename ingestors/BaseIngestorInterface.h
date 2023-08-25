#ifndef BASE_INGESTOR_INTERFACE_H
#define BASE_INGESTOR_INTERFACE_H

#include <vector>

template<typename T>
class BaseIngestorInterface {
    public:
        BaseIngestorInterface() {}
        virtual T readOneRow() = 0;
        virtual vector<T> readNRows(int numRows) = 0;
        // TODO(hanan-li): figure out custom iterators.
};

#endif