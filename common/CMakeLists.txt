cmake_minimum_required(VERSION 3.13)
project(common C CXX)
set(CMAKE_CXX_STANDARD 11)

file(GLOB SOURCES
	${PROJECT_SOURCE_DIR}/*.cpp
)

add_library(${PROJECT_NAME}
	STATIC
	${SOURCES}
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
	${PROJECT_SOURCE_DIR}
	${OpenCV_INCLUDE_DIRS}
)
