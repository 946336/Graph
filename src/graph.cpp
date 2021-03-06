#ifndef GRAPH_GRAPH_946336_CPP
#define GRAPH_GRAPH_946336_CPP

#include "../include/graph.h"
#include "../modules/log/include/log.h"

#include <iostream>
#include <stdexcept>

#include <sstream> // std::ostringstream
#include <algorithm>

#include <unordered_map>
#include <iterator>

// Used to build log/exception messages
// This feels clunky an is probably a bad idea, but I'm not sure where a
// better place would be
// At least it's static, but definitely not thread-safe
static std::ostringstream builder;

// ------------------------------------------------------------------------
//                              Helpers
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Exception message builders
// ------------------------------------------------------------------------

template<typename T, class Compare, class Alloc>
void Graph<T, Compare, Alloc>::notMyNode(const Graph<T, Compare, Alloc>::Node &node) const
{
    builder.flush();
    builder << "Origin node " << node.data << " not found in graph "
            << this->name() << std::endl;
}

template<typename T, class Compare, class Alloc>
void Graph<T, Compare, Alloc>::noSuchNode(const Graph<T, Compare, Alloc>::Node &node) const
{
    builder.flush();
    builder << "Node " << node.data
            << " is no longer valid in graph " << this->name() << std::endl;
}

template<typename T, class Compare, class Alloc>
bool Graph<T, Compare, Alloc>::isValid(const Graph<T, Compare, Alloc>::Node &node,
                                const bool throw_on_error) const
{
    // Do the nodes think they belong to me?
    if (&node.parent() != this) {
        if (!throw_on_error) return false;

        this->notMyNode(node);
        ERROR(this->log, builder.str());
        throw NotMyNode(builder.str());
    }

    // Do I think I have such nodes?
    if (!node.active or node.uid >= this->nodes.size()
            or !this->nodes[node.uid].active) {
        if (!throw_on_error) return false;

        this->noSuchNode(node);
        ERROR(this->log, builder.str());
        throw NoSuchNode(builder.str());
    }

    return true;
}

template<typename T, class Compare, class Alloc>
std::vector<typename Graph<T, Compare, Alloc>::Node>&
Graph<T, Compare, Alloc>::listFor(const size_t uid)
{
    return this->adjacency_list.at(uid);
}

// ------------------------------------------------------------------------
// Graph implementation
// ------------------------------------------------------------------------

template<typename T, class Compare, class Alloc>
Graph<T, Compare, Alloc>::Graph(const Logger::Level l, const std::string &name,
                         const bool no_newlines, std::ostream &sink)
    : log(l, name, no_newlines, sink)
{
    // Nothing to do here
}

template<typename T, class Compare, class Alloc>
Graph<T, Compare, Alloc>::Graph::Graph(const Graph &src,
                                       const std::string &name)
{
    this->adjacency_list = src->adjacency_list;

    // Duplicated nodes must correctly track their parents
    this->nodes.reserve(src->nodes.length());
    for (const auto &n : src->nodes) {
        this->nodes.emplace_back(n.data, *this, n.uid);
    }

    this->log = Logger(src.log, name.empty() ? src.log.name() : name);
}

template<typename T, class Compare, class Alloc>
const std::string& Graph<T, Compare, Alloc>::name() const
{
    return this->log.name();
}

template<typename T, class Compare, class Alloc>
Graph<T, Compare, Alloc>::~Graph()
{
    // Nothing to do here
}

// Extends the outer adjacency list by one and adds an extra node
template<typename T, class Compare, class Alloc>
Graph<T, Compare, Alloc>& Graph<T, Compare, Alloc>::add(const T& value)
{
    size_t n_nodes = this->nodes.size();

    this->nodes.emplace_back(value, *this, n_nodes);
    this->adjacency_list.emplace_back(); // Ensure that the corresponding
                                         // adjacency list entry exists

    return *this;
}

template<typename T, class Compare, class Alloc>
void Graph<T, Compare, Alloc>::print()
{
    INFO(this->log, "Graph: ") << this->name() << '\n';

    for (size_t node_id = 0; node_id < this->nodes.size();
            ++node_id) {

        if (!this->nodes.at(node_id).active) continue;

        INFO(this->log, "Adjacency list for node ") <<
            this->nodes.at(node_id).data << " (" << node_id << "):\n";
        INFO(this->log, "");

        for (size_t endpoint = 0;
                endpoint < this->adjacency_list.at(node_id).size();
                ++endpoint) {

            if (!this->nodes.at(endpoint).active) continue;

            this->log << this->nodes.at(endpoint).data << ", ";
        }
        this->log << "\n";
    }

    INFO(this->log, "---------\n");
}

template<typename T, class Compare, class Alloc>
typename Graph<T, Compare, Alloc>::Node
Graph<T, Compare, Alloc>::find(const Graph::predicate pred) const
{
    for (auto const &node : this->nodes) {
        if (node.active and pred(node.data)) {
            return node;
        }
    }

    // By this point, we've failed find a node satisfying the predicate
    ERROR(this->log, NoSuchNode::default_msg) << '\n';
    throw NoSuchNode(NoSuchNode::default_msg);
}

// This is a directed graph by default. Overload/inherit for undirected?
template<typename T, class Compare, class Alloc>
Graph<T, Compare, Alloc>& Graph<T, Compare, Alloc>::connect(
        const Graph<T, Compare, Alloc>::Node &from,
        const Graph<T, Compare, Alloc>::Node &to)
{
    // Basically asserting
    this->isValid(from);
    this->isValid(to);

    // Do nothing if the edge exists
    for (const auto &dest : this->adjacency_list[from.uid]) {
        if (dest == to.uid) return *this;
    }

    // Add edge if it doesn't already exist
    this->adjacency_list[from.uid].emplace_back(to.uid);

    return *this;
}

// This is a directed graph by default. Overload/inherit for undirected?
template<typename T, class Compare, class Alloc>
Graph<T, Compare, Alloc>& Graph<T, Compare, Alloc>::disconnect(
        const Graph<T, Compare, Alloc>::Node &from,
        const Graph<T, Compare, Alloc>::Node &to)
{
    this->isValid(from);
    this->isValid(to);

    // Remove edge is it exists

    // Find the edge
    auto where = this->adjacency_list[from.uid].find(to.uid);

    if (where == this->adjacency_list[from.uid].end()) {
        return *this;
    }

    auto end = this->adjacency_list[from.uid].end();
    --end; // Otherwise it points one off the end

    std::iter_swap(where, end);

    this->adjacency_list.erase(end, this->adjacency_list[from.uid].end());

    return *this;
}

template<typename T, class Compare, class Alloc>
typename Graph<T, Compare, Alloc>::vector
Graph<T, Compare, Alloc>::neighborsOf(const Graph<T, Compare, Alloc>::Node &node) const
{
    this->isValid(node);

    // This shouldn't ever go OOB on adjacency_list
    std::vector<size_t> ids = this->adjacency_list[node.uid];
    typename Graph<T, Compare, Alloc>::vector neighbors;

    for (const auto &i : ids) {
        if (this->nodes[i].active) {
            /* neighbors.emplace_back(this->nodes[i].data, *this, i); */
            neighbors.push_back(this->nodes[i]);
        }
    }

    return neighbors;
}

// Removes _all_ matching nodes
template<typename T, class Compare, class Alloc>
Graph<T, Compare, Alloc>& Graph<T, Compare, Alloc>::remove(const T& data)
{
    for (auto& n: this->nodes) {
        if (n.active and n.data == data) {
            n.active = false;
        }
    }
    return *this;
}

template<typename T, class Compare, class Alloc>
Graph<T, Compare, Alloc>&
Graph<T, Compare, Alloc>::remove(const Graph<T, Compare, Alloc>::Node &node)
{
    // Does the node think it belongs to me?
    if (&node.parent() != this) {
        this->notMyNode(node);
        ERROR(this->log, builder.str());
        throw NotMyNode(builder.str());
    }
    // Do I think I have this node?
    if (!node.active or node.uid >= this->nodes.size()
            or !this->nodes[node.uid].active) {
        this->noSuchNode(node);
        ERROR(this->log, builder.str());
        throw NoSuchNode(builder.str());
    }

    using Node = Graph<T, Compare, Alloc>::Node;

    /* this->nodes[node.uid].active = node.active = false; */
    this->nodes[node.uid] = Node(node, Node::INACTIVE);
    return *this;
}

template<typename T, class Compare, class Alloc>
Graph<T, Compare, Alloc>& Graph<T, Compare, Alloc>::Node::parent() const {
    return this->parent_.get();
}

// Guarantee: If an exception thrown, the container is not changed
template<typename T, class Compare, class Alloc>
Graph<T, Compare, Alloc>&
Graph<T, Compare, Alloc>::remove(const std::vector< Graph<T, Compare, Alloc>::Node > &nodes)
{
    // Check that all nodes are valid
    for (auto &node: nodes) {
        // Does the node think it belongs to me?
        if (&node.parent() != this) {
            this->notMyNode(node);
            ERROR(this->log, builder.str());
            throw NotMyNode(builder.str());
        }
        // Do I think I have this node?
        if (!node.active or node.uid >= this->nodes.size()
                or !this->nodes[node.uid].active) {
            this->noSuchNode(node);
            ERROR(this->log, builder.str());
            throw NoSuchNode(builder.str());
        }
    }

    for (auto &node: nodes) {
        this->nodes[node.uid].active = node.active = false;
    }
}

template<typename T, class Compare, class Alloc>
Graph<T, Compare, Alloc>& Graph<T, Compare, Alloc>::cleanup()
{
    // Idea: Atomically reassign node uids
    //      Can be done iteratively in-place (I think)

    // This is spares, so we'll go with a map. Maybe a vector would be better,
    // but we'd likely be wasting a decent amount of space
    std::unordered_map< size_t, size_t > uid_changes;

    // Move all "deleted" nodes to the end
    auto nodes_end = std::remove_if(this->nodes.begin(),
                                    this->nodes.end(),
                                    [](typename Graph<T, Compare, Alloc>::Node &n) {
                                        return !n.active;
                                    });

    if (std::distance(nodes_end, this->nodes.end()) == 0) {
        return *this;
    }

    // Delete them, since we won't need them anymore
    this->nodes.erase(nodes_end, this->nodes.end());

    // Scan for changes: mismatches between index and node uid
    //      If an id has an entry in uid_changes, it is old -> new
    // Update node uids to reflect their new positions

    size_t n_nodes = this->nodes.size();
    for (size_t i = 0; i < n_nodes; ++i) {
        if (i != this->nodes[i].uid) {
            uid_changes[this->nodes[i].uid] = i;
            this->nodes[i].uid = i; // Enforce the uid <-> index property
        }
    }

    // -------------------------------
    //    this->nodes is now up to date
    // -------------------------------

    auto forward  = this->adjacency_list.begin();

    // Iterator at the next element to be swapped forward
    auto backward = this->adjacency_list.end();
    --backward;

    // Remove adjacency lists for dead nodes
    for (size_t i = 0; std::distance(forward, backward) >= 0;
            ++i, forward += 1) {
        // Swap if found to have a change
        if (uid_changes.find(i) != uid_changes.end()) {
            --backward; // Why does this have to come first?
                        // Is this a classic 0-based vs 1-based problem?
            std::iter_swap(forward, backward);
        }
    }

    /* forward += 1; // Forward points to the first element to be deleted */

    if (std::distance(forward, this->adjacency_list.end()) != 0) {
        this->adjacency_list.erase(forward, this->adjacency_list.end());
    }

    // ----------------------------------------------------------------------
    // Adjacency lists removed, but contents not updated to reflect changes
    // ----------------------------------------------------------------------

    // Scan adjacency lists, changing target node uids according to recorded
    // changes

    // Per list
    size_t size = this->adjacency_list.size();
    for (size_t list_idx = 0; list_idx < size; ++list_idx) {
        // How many edges do we have out of this node?
        size_t degree = this->adjacency_list[list_idx].size();
        // Per adjacency entry in the list
        for (size_t entry = 0; entry < degree; ++entry) {
            size_t uid = this->adjacency_list[list_idx][entry];

            this->adjacency_list[list_idx][entry] =
                ((uid_changes.find(uid) != uid_changes.end()) ?
                    uid_changes[uid] : uid);
        }
    }

    return *this;
}

template<typename T, class Compare, class Alloc>
bool Graph<T, Compare, Alloc>::areAdjacent(
        const Graph<T, Compare, Alloc>::Node &lhs,
        const Graph<T, Compare, Alloc>::Node &rhs) const
{
    this->isValid(lhs);
    this->isValid(rhs);

    if (this->adjacency_list[lhs].find(rhs) !=
            this->adjacency_list[lhs].end()) {
        return true;
    }

    if (this->adjacency_list[lhs].find(rhs) !=
            this->adjacency_list[rhs].end()) {
        return true;
    }

    return true;
}

template<typename T, class Compare, class Alloc>
bool Graph<T, Compare, Alloc>::isMine(
        const Graph<T, Compare, Alloc>::Node &node) const
{
    return this->isValid(node, false);
}

template<typename T, class Compare, class Alloc>
Vector<T, Alloc>
Graph<T, Compare, Alloc>::filter(const Graph::predicate pred) const
{
    Vector<T, Alloc> data;

    for (const Graph<T, Compare, Alloc>::Node &n : this->nodes) {
        if (pred(n.data)) {
            data.emplace_back(data);
        }
    }

    return data;
}

template<typename T, class Compare, class Alloc>
typename Graph<T, Compare, Alloc>::vector
Graph<T, Compare, Alloc>::mask(const Graph::predicate pred)
{
    Graph<T, Compare, Alloc>::vector nodes;

    for (const Graph<T, Compare, Alloc>::Node &n : this->nodes) {
        if (pred(n.data)) {
            /* nodes.emplace_back(n.data, *this, n.uid); */
            nodes.push_back(Node(n.data, *this, n.uid));
        }
    }

    return nodes;
}

#endif

