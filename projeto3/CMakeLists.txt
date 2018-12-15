project(crawler)
cmake_minimum_required(VERSION 2.8)

# Point to our own cmake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)

find_package(PkgConfig REQUIRED)

# Add global definitions
add_definitions("-Wall")

# Add cpr
add_subdirectory(cpr)
include_directories(${CPR_INCLUDE_DIRS})

#Add boost
find_package( Boost 1.65.1 COMPONENTS regex mpi)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

#Add MPI
FIND_PACKAGE(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

# generate sequential ".ex"
add_executable(controller controller.cpp produto.cpp)
target_link_libraries(controller ${CPR_LIBRARIES} ${Boost_LIBRARIES} ${MPI_LIBRARIES})
set_target_properties(controller PROPERTIES COMPILE_FLAGS "-lboost_regex -lboost_mpi")