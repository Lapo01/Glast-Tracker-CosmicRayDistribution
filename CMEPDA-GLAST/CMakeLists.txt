cmake_minimum_required(VERSION 3.0)
project(Glast-Tracker)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY SharedLibs)
set(CMAKE_SOURCE_DIR Core/LibrariesSources)

message(STATUS "Looking for ROOT")
find_package(ROOT REQUIRED)
message(STATUS "ROOT ${ROOT_VERSION} found at ${ROOT_BINDIR}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${ROOT_INCLUDE_DIRS})
include_directories(Core/Headers)



add_library(GeneralUtils SHARED ${CMAKE_SOURCE_DIR}/GeneralUtils.cpp )
add_library(UtilsCreateTree SHARED ${CMAKE_SOURCE_DIR}/UtilsCreateTree.cpp )
add_library(UtilsRetina SHARED ${CMAKE_SOURCE_DIR}/UtilsRetina.cpp )

