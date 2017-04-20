#include <iostream>

#include "log.h"

int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    Logger l(Logger::Level::DEBUG, "Hello", true, std::cerr);

    INFO(l, "HELLO") << " THERE" << '\n';
    INFO(l, "AAAAAAAAAAAA") << 3 << '\n';

    return 0;
}
