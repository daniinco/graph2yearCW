cmake_minimum_required(VERSION 3.24)
project(graph_for_rework)

set(CMAKE_CXX_STANDARD 17)

add_executable(graph_for_rework main.cpp read_write.cpp graph_struct.cpp read_write.h graph_struct.h search_occur.h search_occur.cpp copy_functions.h copy_functions.cpp classic_extension.h classic_extension.cpp percent_funcs.h percent_funcs.cpp dsu.h dsu.cpp percent_extension.h percent_extension.cpp compile_extensions.h compile_extensions.cpp)
