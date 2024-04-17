# Top level CMakeLists.txt to build the Communicating Sequential Elements Demo
cmake_minimum_required(VERSION 3.13)

PROJECT(cseDemo)

MESSAGE(${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)

add_subdirectory(common)
add_subdirectory(demoseq)
add_subdirectory(demopar)
add_subdirectory(democse)
