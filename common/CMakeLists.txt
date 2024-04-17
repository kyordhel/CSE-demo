cmake_minimum_required(VERSION 3.13)
project(common C CXX)
set(CMAKE_CXX_STANDARD 11)

# We need OpenCV
find_package(OpenCV REQUIRED)

file(GLOB SOURCES
	${PROJECT_SOURCE_DIR}/*.cpp
)
MESSAGE("Sources: ${SOURCES}")

add_library(${PROJECT_NAME}
	STATIC
	${SOURCES}
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
	${PROJECT_SOURCE_DIR}
	${OpenCV_INCLUDE_DIRS}
)
