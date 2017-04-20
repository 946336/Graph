#ifndef GRAPH_GRAPH_946336_H
#define GRAPH_GRAPH_946336_H

#include <functional>
#include <vector>
#include <memory>

#include <iostream>
#include <stdexcept>

#include "../modules/log/include/log.h"

#include "graph_exceptions.h"
#include "graph_vector.h"

// -----------------------------------------------------------------------
// Data types must have the following operations defined and publicly
// available:
//      operator< (or provide a comparator)
//      operator==
//      Copy constructor
//      (???) Default constructor
// -----------------------------------------------------------------------

// allocator for elements, not nodes
template <typename T, class Compare = std::less<T> ,
         class Alloc = std::allocator<T> >
class Graph
{
public:
    Graph(const Logger::Level l = Logger::WARNING,
          const std::string &name = "Graph", const bool no_newlines = true,
          std::ostream &sink = std::cerr);
    Graph(const Graph &src, const std::string &new_name = "");
    ~Graph();

    using predicate = bool (*)(T data);

protected:
    // Node - internal type holding data and metadata
    class Node
    {
    public:
        Node(const T &data_, Graph<T, Compare, Alloc> &parent,
             const size_t id);
        /* Node(T &data_, Graph<T, Compare, Alloc> &parent, */
        /*      const size_t id); */
        ~Node();

        Graph<T, Compare, Alloc>& parent() const;

        friend class Graph<T, Compare, Alloc>;

        T data;

    protected:
        // If you're feeling particularly verbose
        enum { ACTIVE = true, INACTIVE = false } STATE;

        // Fabulous. Let the jank flow through you
        Node(const Node &src, const bool active);
        /* Node(Node &src, const bool active); */

        // We use the node's position in the internal list of nodes as its uid
        size_t uid;
        bool active;
        mutable std::reference_wrapper< Graph<T, Compare, Alloc> > parent_;
    };

    // Consider using a set instead? IIRC it's been shown in the real world
    // that scanning a vector is faster than using a set for up to a few
    // thousand elements
    std::vector< std::vector< size_t > > adjacency_list;
    std::vector< Node > nodes;

    mutable Logger log; // This isn't part of object state

    // ----------------------------------------------------------------
    // Utility functions

    // Build error strings
    // NEDDSWORK - Maybe use a different naming convention?
    void noSuchNode(const Node &n) const;
    void notMyNode(const Node &n) const;

    // Provided as an assertion
    // By default, throws NotMyNode or NoSuchNode on failure.
    // If throw_on_error is false, returns false instead
    // Note: No messages are logged when throw_on_error is false
    bool isValid(const Node &n, const bool throw_on_error = true) const;

    // Grabs the adjacency list associated with the node identified by uid
    std::vector< Node >& listFor(const size_t uid);

public:

    // Convenience
    class vector : public Vector<Node, Alloc> {
    public:
        // Unwrap all nodes, exposing their elements
        Vector<T, Alloc> unwrap() const
        {
            Vector<T, Alloc> res;

            for (const auto &node : *this) {
                res.emplace_back(node.data);
            }

            return res;
        }
    };


    // Convenience during development
    void print();
    Graph<T, Compare, Alloc>& rename(const std::string &rename);
    const std::string& name() const;

    Graph<T, Compare, Alloc>& add(const T &value);

    // Remove all nodes with this value
    // Does NOT throw an exception if no nodes are removed
    Graph<T, Compare, Alloc>& remove(const T &value);

    // Remove the named node from the graph. The argument node is invalidated
    // after this call completes.
    // If the node does not belong to the graoh, a NotMyNode exception is
    //      thrown
    // If the node does not exist or is already invalid, a NoSuchNode exception
    //      is thrown
    // If an exception is thrown, neither the argument nor the container are
    // changed.
    Graph<T, Compare, Alloc>& remove(const Node &node);

    // Remove the nodes from the graph. The argument nodes are invalidated
    // after this call completes.
    // If an exception is thrown, neither the argument nor the container are
    // changed.
    Graph<T, Compare, Alloc>& remove(const std::vector< Node > &nodes);

    // WARNING: This invalidates any and all references to the nodes of this
    // graph.
    // Cleans up deleted/inactive nodes, shrinking the memory footprint of the
    // graph.
    // As you can imagine, this is slow and expensive. Use with caution
    Graph<T, Compare, Alloc>& cleanup();

    // Finds a node holding data that fulfills the predicate. find is not
    // guaranteed to find that same node each time.
    // Raises RuntimeError when no node satisfies the predicate
    Node find(const predicate pred) const;

    // Use filter methods to extract data from the graph
    // Use mask methods to extract nodes from the graph

    Vector<T, Alloc> filter(const predicate pred) const;

    // Can't be const. RIP
    vector mask(const predicate pred);

    // Creates an edge between two nodes (directed)
    // It is an error to pass nodes that do not exist in the graph
    // If the nodes are already connected, the container is not modified
    Graph<T, Compare, Alloc>& connect(const Node &from, const Node &to);

    // Removes the directed edge between the nodes
    // It is an error to pass nodes that do not exist in the graph
    // If the nodes are not connected, the container is not modified
    Graph<T, Compare, Alloc>& disconnect(const Node &from, const Node &to);

    // Finds all immediate neighbors of the node.
    // Throws NotMyNode
    vector neighborsOf(const Node &node) const;

    // Are the nodes adjacent?
    // Throws NotMyNode
    bool areAdjacent(const Node &lhs, const Node &rhs) const;

    // Determines if the node belongs to this graph
    bool isMine(const Node &node) const;
};

// I prefer to keep implementations in separate files
#include "../src/graph.cpp"
#include "../src/node.cpp"

#endif

