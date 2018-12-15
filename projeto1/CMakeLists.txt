project(visualizador)
cmake_minimum_required(VERSION 2.8)

# Point to our own cmake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)

find_package(PkgConfig REQUIRED)
# Find SDL2

pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_gfx REQUIRED SDL2_gfx)

# Add global definitions
add_definitions("-Wall")
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_gfx_INCLUDE_DIR})

# compile flags
SET(SIMDFLAG "-ffast-math -ftree-vectorize -mavx")
SET(OMPFLAG "-fopenmp")

add_executable(visualizadorOMP main.cpp visualizador.cpp impactCalc.cpp)
target_link_libraries(visualizadorOMP ${SDL2_LIBRARIES} ${SDL2_gfx_LIBRARIES})
SET_TARGET_PROPERTIES(visualizadorOMP PROPERTIES COMPILE_FLAGS ${OMPFLAG} LINK_FLAGS ${OMPFLAG})

add_executable(parallelOMP testParallel.cpp impactCalc.cpp)
target_link_libraries(parallelOMP ${SDL2_LIBRARIES} ${SDL2_gfx_LIBRARIES})
SET_TARGET_PROPERTIES(parallelOMP PROPERTIES COMPILE_FLAGS ${OMPFLAG} LINK_FLAGS ${OMPFLAG})

add_executable(visualizadorSIMD main.cpp visualizador.cpp impactCalc.cpp)
target_link_libraries(visualizadorSIMD ${SDL2_LIBRARIES} ${SDL2_gfx_LIBRARIES})
SET_TARGET_PROPERTIES(visualizadorSIMD PROPERTIES COMPILE_FLAGS ${SIMDFLAG})

add_executable(parallelSIMD testParallel.cpp impactCalc.cpp)
target_link_libraries(parallelSIMD ${SDL2_LIBRARIES} ${SDL2_gfx_LIBRARIES})
SET_TARGET_PROPERTIES(parallelSIMD PROPERTIES COMPILE_FLAGS ${SIMDFLAG})

add_executable(visualizadorNORMAL main.cpp visualizador.cpp impactCalc.cpp)
target_link_libraries(visualizadorNORMAL ${SDL2_LIBRARIES} ${SDL2_gfx_LIBRARIES})

add_executable(parallelNORMAL testParallel.cpp impactCalc.cpp)
target_link_libraries(parallelNORMAL ${SDL2_LIBRARIES} ${SDL2_gfx_LIBRARIES})
