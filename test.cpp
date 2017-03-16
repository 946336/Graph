#include "graph.h"

int main(int argc, char** argv)
{
    (void) argc;
    (void) argv;

    Graph<int> g(Logger::Level::INFO);
    g.add(3);
    g.add(5);
    g.add(157286);

    Graph<int> h;
    h.add(4);

    auto other = h.find([](int n){return n == 4;});

    auto from = g.find([](int n){return n == 5;});
    auto to   = g.find([](int n){return n == 3;});
    auto big  = g.find([](int n){return n == 157286;});

    g.connect(from, to);
    g.connect(from, from);

    g.remove(big);
    /* g.remove(from); */

    g.cleanup();
    g.print();

    /* try { */
    /*     std::cout << g.find([](int candidate) { */
    /*                 return candidate == 7; */
    /*             }).data << std::endl; */
    /* } catch (NoSuchNode e) { */
    /*     std::cout << e.what() << ' ' << g.name() << std::endl; */
    /* } */

    /* try { */
    /*     for (const auto& n : g.neighborsOf(other)) { */
    /*         std::cout << n.data << ", "; */
    /*     } */
    /*     std::cout << std::endl; */
    /* } catch (NoSuchNode e) { */
    /*     std::cout << e.what() << std::endl; */
    /* } catch (NotMyNode e) { */
    /*     std::cout << e.what() << std::endl; */
    /* } */

    /* g.print(); */

    /* g.remove(to); */

    /* g.print(); */

    return 0;
}

