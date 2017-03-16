#include <iostream>

#include "log.h"

int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    Logger l(Logger::Level::DEBUG, "Hello", true, std::cerr);

    LOG(l, "HELLO") << " THERE" << '\n';
    LOG(l, "AAAAAAAAAAAA") << 3 << '\n';

    return 0;
}
