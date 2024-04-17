# Top level CMakeLists.txt to build the Communicating Sequential Elements Demo
cmake_minimum_required(VERSION 3.13)

PROJECT(cseDemo)

add_subdirectory(seq)
add_subdirectory(par)
add_subdirectory(cse)
