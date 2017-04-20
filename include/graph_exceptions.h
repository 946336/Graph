#ifndef GRAPH_EXCEPTIONS_946336
#define GRAPH_EXCEPTIONS_946336

#include <stdexcept>

// -----------------------------------------------------------------------
// Exceptions defined for graphs
// default messages do not contain trailing newlines or trailing spaces
// -----------------------------------------------------------------------

class NoSuchNode : public std::runtime_error
{
public:
    NoSuchNode(const std::string &what = default_msg)
        : runtime_error(what) {};
    NoSuchNode(const char *what = default_msg)
        : runtime_error(what) {};

    static const char * const default_msg;
};

class NotMyNode : public std::runtime_error
{
public:
    NotMyNode(const std::string &what = default_msg)
        : runtime_error(what) {};
    NotMyNode(const char *what = default_msg)
        : runtime_error(what) {};

    static const char * const default_msg;
};

#endif

