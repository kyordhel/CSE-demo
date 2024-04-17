cmake_minimum_required(VERSION 3.13)
project(demo-seq C CXX)
set(CMAKE_CXX_STANDARD 11)

# We need OpenCV
find_package(OpenCV REQUIRED)

MESSAGE("SrcDir: ${PROJECT_SOURCE_DIR}")

# file(GLOB "${PROJECT_NAME}_SRC"
file(GLOB SOURCES
	${PROJECT_SOURCE_DIR}/*.cpp
)
MESSAGE("Sources: ${SOURCES}")

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
