cmake_minimum_required(VERSION 3.13)
project(demo-cse C CXX)
set(CMAKE_CXX_STANDARD 11)

file(GLOB SOURCES
	${PROJECT_SOURCE_DIR}/*.cpp
)

add_executable(${PROJECT_NAME}
	${SOURCES}
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
	common
	${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
	common
	pthread
	${OpenCV_LIBS}
)
