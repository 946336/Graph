cmake_minimum_required(VERSION 3.0)
project(Graph)

# ==========================================================================
# Include things

# Let filenames be base filenames instead of the full paths that cmake likes
# to use
include(basename.cmake)

# ==========================================================================
# Setup env
# ==========================================================================

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ==========================================================================
# Set flags
# ==========================================================================

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic -std=c++14 -g -O2")

# ==========================================================================
# Include headers
# ==========================================================================

include_directories(include)

# ==========================================================================
# Include from modules
# ==========================================================================

# Logging
include_directories(modules/log)

# ==========================================================================
# Define targets
# ==========================================================================

add_executable(test1 src/test.cpp modules/log/log.cpp src/graph_exceptions.cpp)
define_file_basename_for_sources(test1)

add_library(graph SHARED src/graph_exceptions.cpp modules/log/log.cpp)

