
CXX ?= g++
CXXFLAGS ?= -Wall -Wextra -Werror -pedantic
CXXFLAGS += -std=c++14 -g

LOG_DIR = log
LOG_DEPS = $(LOG_DIR)/log.h $(LOG_DIR)/log.cpp
# Non-header log components
LOG_COMPONENTS = $(LOG_DIR)/log.o

# Non-header graph dependencies
GRAPH_OBJS = graph_exceptions.o

all: test
.PHONY: all

test: test.cpp graph.h graph.cpp node.cpp graph_exceptions.o $(LOG_COMPONENTS)
	$(CXX) $(CXXFLAGS) $< $(GRAPH_OBJS) $(LOG_COMPONENTS) -o $@

$(LOG_DIR):
	$(MAKE) -C $@

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) $< -c -o $@

clean:
	rm -f *.o test test_vector
	$(MAKE) -C $(LOG_DIR) clean

.PHONY: log
.PHONY: clean

