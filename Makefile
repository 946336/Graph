
CXX ?= g++
CXXFLAGS += -Wall -Wextra -Werror -pedantic -std=c++14 -g

LOG_DIR = log

# Graph dependencies
GRAPH_OBJS = log.o graph_exceptions.o

.PHONY: log
.PHONY: clean

all: test test_vector log

test: test.cpp graph.h graph.cpp node.cpp log.o graph_exceptions.o
	$(CXX) $(CXXFLAGS) $< $(GRAPH_OBJS) -o $@

test_vector: test_vector.cpp graph_vector.h graph_vector.cpp
	$(CXX) $(CXXFLAGS) $^ -o test_logger

log:
	$(MAKE) -C $(LOG_DIR)

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) $< -c -o $@

clean:
	rm -f *.o test test_logger test_vector

