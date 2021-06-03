cmake_minimum_required(VERSION 3.10)
project(librlx)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(yaml-cpp REQUIRED)
include_directories(lib ${YAML_CPP_INCLUDE_DIRS})

add_subdirectory(cli)
add_subdirectory(curl)
add_subdirectory(path)
add_subdirectory(utils)
add_subdirectory(instlr)
add_subdirectory(sys)