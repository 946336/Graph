
CXX ?= g++
CXXFLAGS += -Wall -Wextra -Werror -pedantic -std=c++14 -g

all: log.o test_logger

test_logger: test_logger.cpp log.o
	$(CXX) $(CXXFLAGS) $^ -o test_logger

clean:
	rm -f *.o test_logger

.PHONY: clean
.PHONY: all

