cmake_minimum_required(VERSION 2.8)

set(PREFIX ../)

file(GLOB_RECURSE SOURCE ${PREFIX}/src/*.cpp)
file(GLOB_RECURSE CONVEX ${PREFIX}/box2d/Contributions/Utilities/ConvexDecomposition/*.cpp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PREFIX})

set(CMAKE_MODULE_PATH ./)
find_package(SFML REQUIRED)

link_directories(${CMAKE_SOURCE_DIR}/${PREFIX}/box2d/Box2D/Build/Box2D/)
include_directories(${PREFIX}/include/ ${PREFIX}/gpc ${PREFIX}/box2d/Box2D/ ${PREFIX}/box2d/Box2D/Box2D/ ${PREFIX}/box2d/Box2D/Box2D/Common/ ${PREFIX}/box2d/Contributions/Utilities/ConvexDecomposition ${SFML_INCLUDE_DIR})

add_executable(main ${SOURCE} ${CONVEX} ${PREFIX}/gpc/gpc.c)
target_link_libraries(main Box2D ${SFML_LIBRARY})
