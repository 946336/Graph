# Graph

An early pass at a sparse graph template in C++.

## Building

From the root directory of the project:

    cmake .
    make

produces `libgraph.so` in `lib/` and a very simple test `test1` in `test/bin/`

## Disclaimer

This is a less-then-optimal implementation in many ways, chief of which is
unnecessary complexity brought on by holding extraneous information.

