#ifndef GRAPH_NODE_946336
#define GRAPH_NODE_946336

#include "graph.h"

// Actually using Alloc is apparently painful and not quite possible
// right now

template<typename T, class Compare, class Alloc>
Graph<T, Compare, Alloc>::Node::Node(const T &value,
                              Graph<T, Compare, Alloc>& parent,
                              const size_t id)
    /* : data(Alloc(value)), uid(id), active(true), parent_(parent) */
    : data(value), uid(id), active(true), parent_(parent)
{
    // Nothing to do here
}

/* template<typename T, class Compare, class Alloc> */
/* Graph<T, Compare, Alloc>::Node::Node(T &value, */
/*                               Graph<T, Compare, Alloc>& parent, */
/*                               const size_t id) */
/*     : data(Alloc(value)), uid(id), active(true), parent_(parent) */
/* { */
    // Nothing to do here
    /* this->data = std::move(Alloc(value)); */
/* } */

template<typename T, class Compare, class Alloc>
Graph<T, Compare, Alloc>::Node::Node(const Graph<T, Compare, Alloc>::Node &src,
                              const bool active_)
    : data(src.data), uid(src.uid), active(active_), parent_(src.parent_)
{
    // Nothing to do here
}

/* template<typename T, class Compare, class Alloc> */
/* Graph<T, Compare, Alloc>::Node::Node(Graph<T, Compare, Alloc>::Node &src, */
/*                               const bool active_) */
/*     : data(Alloc(src.data)), uid(src.uid), active(active_), parent_(src.parent_) */
/* { */
/*     // Nothing to do here */
/* } */

template<typename T, class Compare, class Alloc>
Graph<T, Compare, Alloc>::Node::~Node()
{
    // Nothing to do here
}

#endif

