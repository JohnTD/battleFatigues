cmake_minimum_required(VERSION 2.8)
project(battle-fatigues)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

find_package(OpenCV REQUIRED)

include_directories(${PROJECT_SRC_DIR}/include, ${OpenCV_INCLUDE_DIRS})
link_directories(${PROJECT_SRC_DIR}/lib)

aux_source_directory(. SRC_LIST)
add_executable(battle-fatigues ${SRC_LIST})
target_link_libraries(battle-fatigues ${OpenCV_LIBS})
