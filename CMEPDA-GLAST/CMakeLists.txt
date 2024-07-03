cmake_minimum_required(VERSION 3.0)
project(Glast-Tracker)
include(CTest)
set(CMAKE_CXX_STANDARD 17)







if(NOT DEFINED ROOTSYS AND DEFINED ENV{ROOTSYS})
  file(TO_CMAKE_PATH $ENV{ROOTSYS} ROOTSYS)
  list(INSERT CMAKE_PREFIX_PATH 0 ${ROOTSYS})
endif()

if(DEFINED ROOTSYS)
  list(INSERT CMAKE_PREFIX_PATH 0 ${ROOTSYS})
endif()





message(STATUS "Looking for ROOT")
find_package(ROOT REQUIRED CONFIG)
message(STATUS "ROOT ${ROOT_VERSION} found at ${ROOT_BINDIR}")



set(CMAKE_LIBRARY_OUTPUT_DIRECTORY SharedLibs)
set(CMAKE_SOURCE_DIR Core/LibrariesSources)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${ROOT_INCLUDE_DIRS})
include_directories(Core/Headers)
link_directories(${ROOT_LIBRARY_DIRS})



add_definitions(${ROOT_CXX_FLAGS})

cmake_path(GET CMAKE_CURRENT_LIST_FILE PARENT_PATH PARENT_DIR)
cmake_path(GET PARENT_DIR PARENT_PATH PARENT_DIR2)

set(TestDir ${PARENT_DIR2}/tests)
set(CoreDir ${PARENT_DIR}/Core)

message(STATUS ${CoreDir})

set(DataDir ${PARENT_DIR}/Data/Datalif)


add_library(UtilsMC SHARED ${CMAKE_SOURCE_DIR}/UtilsMC.cpp)
add_library(UtilsRectifyTracks SHARED ${CMAKE_SOURCE_DIR}/UtilsRectifyTracks.cpp )
add_library(GeneralUtils SHARED ${CMAKE_SOURCE_DIR}/GeneralUtils.cpp )
add_library(UtilsCreateTree SHARED ${CMAKE_SOURCE_DIR}/UtilsCreateTree.cpp )
add_library(UtilsRetina SHARED ${CMAKE_SOURCE_DIR}/UtilsRetina.cpp )

target_link_libraries(UtilsMC ${ROOT_LIBRARIES})
target_link_libraries(UtilsRectifyTracks ${ROOT_LIBRARIES})
target_link_libraries(GeneralUtils ${ROOT_LIBRARIES})
target_link_libraries(UtilsCreateTree ${ROOT_LIBRARIES})
target_link_libraries(UtilsRetina ${ROOT_LIBRARIES})





add_executable(TestUtilsCreateTree ${TestDir}/TestUtilsCreateTree.cpp)
target_link_libraries(TestUtilsCreateTree UtilsMC)
target_link_libraries(TestUtilsCreateTree UtilsRectifyTracks)
target_link_libraries(TestUtilsCreateTree GeneralUtils)
target_link_libraries(TestUtilsCreateTree UtilsCreateTree)
target_link_libraries(TestUtilsCreateTree UtilsRetina)
add_test(NAME TestUtilsCreateTree COMMAND TestUtilsCreateTree)




