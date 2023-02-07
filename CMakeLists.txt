cmake_minimum_required(VERSION 3.22.2)
project(libos C CXX)
#options
set(BUILD_DOCS OFF)
set(ENABLE_TESTS ON)
#options end

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# Debug mode by default.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DWITH_DEBUG)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    message(INFO " - building for Linux....")
    add_definitions(-DON_LINUX)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "WindowsStore")
    message(INFO " -  building for WindowsStore....")
   add_definitions(-DON_UWP)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    message(INFO " - building for Windows....")
    add_definitions(-DON_WINDOWS)
else()
    message(FATAL_ERROR "system not supported")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions(-DIS_CLANG)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_definitions(-DIS_GCC)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_definitions(-DIS_MSVC)
else()
    message(FATAL_ERROR "compiler not supported")
endif()

add_library(libos ${PROJECT_SOURCE_DIR}/base.cpp)
target_include_directories(libos PRIVATE ${PROJECT_SOURCE_DIR}/Includes STATIC)
include(SourceList)
add_subdirectory(${PROJECT_SOURCE_DIR}/src)

if(${ENABLE_TESTS})
    include(${PROJECT_SOURCE_DIR}/CMake_Build_Platforms/Testing.cmake)
endif()

if(${BUILD_DOCS})
    include(${PROJECT_SOURCE_DIR}/CMake_Build_Platforms/Doxygen.cmake)
endif()
