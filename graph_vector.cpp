#ifndef GRAPH_VECTOR_946336_CPP
#define GRAPH_VECTOR_946336_CPP

#include "graph_vector.h"

// --------------------------------------------------------
// Constructors are supposed to be inherited, so we omit
// them here
// --------------------------------------------------------

template<typename T, class Alloc>
Vector<T, Alloc>::~Vector()
{
    // Nothing to do here
}

template<typename T, class Alloc>
T& Vector<T, Alloc>::find(const Vector<T, Alloc>::predicate pred)
{
    for (auto &elem : *this) {
        if (pred(elem)) return elem;
    }

    throw NotFound("No element satisfying predicate found");
}

template<typename T, class Alloc>
const T& Vector<T, Alloc>::find(const Vector<T, Alloc>::predicate pred) const
{
    return this->find(pred);
}

template<typename T, class Alloc>
Vector<T, Alloc> Vector<T, Alloc>::filter(const predicate pred) const
{
    Vector<T, Alloc> res;

    for (const auto &elem : *this) {
        if (pred(elem)) res.emplace_back(elem);
    }

    return res;
}

#endif

