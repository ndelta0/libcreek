cmake_minimum_required(VERSION 3.15)

project(
    Creek
    VERSION 1.0.0
    DESCRIPTION "Dep-free networking library in C++"
    LANGUAGES CXX
    HOMEPAGE_URL "https://github.com/ndelta0/creek")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(Helpers)

include(FetchContent)

add_subdirectory(src)

option(CREEK_BUILD_DOCS "Build documentation" ON)
if(CREEK_BUILD_DOCS)
    find_package(Doxygen REQUIRED)
    message(STATUS "Building docs")
    add_subdirectory(docs)
endif()

option(CREEK_BUILD_EXAMPLES "Build examples" ON)
if(CREEK_BUILD_EXAMPLES)
    message(STATUS "Building examples")
    add_subdirectory(examples)
endif()