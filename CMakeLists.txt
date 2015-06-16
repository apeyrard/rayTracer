cmake_minimum_required (VERSION 2.8)

project(rayTracer)

option(rayTracer_build_tests "Build unit tests" ON)

set (CMAKE_BUILD_TYPE debug)

set (RAYTRACER_VERSION_MAJOR 0)
set (RAYTRACER_VERSION_MINOR 1)
set (RAYTRACER_VERSION_PATCH 0)
set (RAYTRACER_VERSION ${RAYTRACER_VERSION_MAJOR}.${RAYTRACER_VERSION_MINOR}.${RAYTRACER_VERSION_PATCH})

set(RAYTRACER_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

# Flags
if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-g -fopenmp -O2 -Wall -Wno-sign-compare")
    add_definitions(-std=gnu++0x)
endif()


### Sources ###

# Include directories
set(RAYTRACER_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
include_directories(${RAYTRACER_INCLUDE_DIR})

# Sources
set (RAYTRACER_SOURCE_DIR ./src)
set (RAYTRACER_TESTS_DIR ./tests)
set (RAYTRACER_SOURCES
    ${RAYTRACER_SOURCE_DIR}/vec3.cpp
    ${RAYTRACER_SOURCE_DIR}/ray.cpp
    ${RAYTRACER_SOURCE_DIR}/object.cpp
    ${RAYTRACER_SOURCE_DIR}/sphere.cpp
    #${RAYTRACER_SOURCE_DIR}/plane.cpp
    ${RAYTRACER_SOURCE_DIR}/box.cpp
    ${RAYTRACER_SOURCE_DIR}/main.cpp
)

### Executable ###
set(EXECUTABLE_NAME rayTracer)
set(EXECUTABLE_OUTPUT_PATH ${RAYTRACER_BINARY_DIR})
add_executable(${EXECUTABLE_NAME} ${RAYTRACER_SOURCES})

### library ###
add_library(raytracer ${RAYTRACER_SOURCES})

### Tests ###
if (rayTracer_build_tests)
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
    enable_testing()
    add_executable(rayTracer_test ${RAYTRACER_TESTS_DIR}/test_raytracer.cpp)
    target_link_libraries(rayTracer_test ${GTEST_BOTH_LIBRARIES} raytracer)
    add_test(rayTracer-tests ${CMAKE_SOURCE_DIR}/bin/debug/rayTracer_test)
endif()

LINK_DIRECTORIES(${CMAKE_BINARY_DIR} /libs)

TARGET_LINK_LIBRARIES(rayTracer libgomp.so.1)
TARGET_LINK_LIBRARIES(rayTracer libstdc++.so.6)


#find_package(SFML 2 REQUIRED system window graphics network audio)
#if(SFML_FOUND)
#    include_directories(${SFML_INCLUDE_DIR})
#    target_link_libraries(rayTracer ${SFML_LIBRARIES})
#endif()
