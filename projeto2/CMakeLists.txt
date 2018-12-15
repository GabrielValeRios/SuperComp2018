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
FIND_PACKAGE( Boost 1.65.1 COMPONENTS regex )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

# generate parallel and sequential ".ex"
add_executable(crawler_parallel mainP.cpp crawler_par.cpp produto.cpp)
target_link_libraries(crawler_parallel ${CPR_LIBRARIES} ${Boost_LIBRARIES})
SET_TARGET_PROPERTIES(crawler_parallel PROPERTIES COMPILE_FLAGS "-lpthread -lboost_regex")

add_executable(crawler_sequential mainS.cpp crawler_seq.cpp produto.cpp)
target_link_libraries(crawler_sequential ${CPR_LIBRARIES} ${Boost_LIBRARIES})
SET_TARGET_PROPERTIES(crawler_sequential PROPERTIES COMPILE_FLAGS "-lboost_regex")