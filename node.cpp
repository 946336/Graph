#ifndef GRAPH_NODE_946336
#define GRAPH_NODE_946336

#include "graph.h"

template<typename T, class Compare>
Graph<T, Compare>::Node::Node(const T &value,
                              Graph<T, Compare>& parent,
                              const size_t id)
    : data(value), uid(id), active(true), parent_(parent)
{
    // Nothing to do here
}

template<typename T, class Compare>
Graph<T, Compare>::Node::~Node()
{
    // Nothing to do here
}

#endif

