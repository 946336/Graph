#ifndef GRAPH_VECTOR_946336_H
#define GRAPH_VECTOR_946336_H

#include <vector>
#include <memory>

#include <stdexcept>

class NotFound : public std::runtime_error {
    // Inherit constructors?
    using std::runtime_error::runtime_error;
};

// Extend the vector interface a smidge

template<typename T, class Alloc = std::allocator<T> >
class Vector : public std::vector<T, Alloc> {
public:
    // Inheriting contsructors, hopefully
    using std::vector<T, Alloc>::vector;

    ~Vector();

    using predicate = bool (*)(T data);

    // Find a single element that satisfies the predicate
    // Throws an exception if not found
    T&       find(const predicate pred);
    const T& find(const predicate pred) const;

    // Produce a copy of this, leaving out elements that do not satisfy the
    // predicate. For in-place filter operations, consider std::remove_if
    // from <algorithm>
    Vector filter(const predicate pred) const;

};

#include "../src/graph_vector.cpp"

#endif

