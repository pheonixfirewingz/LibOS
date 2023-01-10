cmake_minimum_required(VERSION 3.22.2)
project(libos C CXX)
#options
set(DEBUG_MODE OFF)
set(BUILD_DOCS OFF)
set(ENABLE_TESTS ON)
#options end
if(${CMAKE_SYSTEM_NAME} MATCHES "WindowsStore")
    set(CMAKE_SYSTEM_ISUWP ON)
else()
    set(CMAKE_SYSTEM_ISUWP OFF)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_COMPILER_NUMBER 0)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_COMPILER_NUMBER 1)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_COMPILER_NUMBER 2)
else()
    message(FATAL_ERROR "compiler not supported")
endif()

configure_file(${PROJECT_SOURCE_DIR}/src/shared/Cmake.h.in ${PROJECT_SOURCE_DIR}/src/shared/Cmake.h @ONLY)
#platform independent code
set(LibOS_Source
${PROJECT_SOURCE_DIR}/src/libOS/Interface/Window.cpp
${PROJECT_SOURCE_DIR}/src/libOS/Interface/Main.cpp
${PROJECT_SOURCE_DIR}/src/libOS/Interface/NetIO.cpp
${PROJECT_SOURCE_DIR}/src/libOS/Interface/FileIO.cpp)
set(LibMedia_Source
${PROJECT_SOURCE_DIR}/src/libMedia/Interface/Render.cpp)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    message(INFO " - building for Linux....")
    include(${PROJECT_SOURCE_DIR}/CMake_Build_Platforms/Linux.cmake)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "WindowsStore")
    message(INFO " -  building for WindowsStore....")
    include(${PROJECT_SOURCE_DIR}/CMake_Build_Platforms/WindowsStore.cmake)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    message(INFO " - building for Windows....")
    include(${PROJECT_SOURCE_DIR}/CMake_Build_Platforms/Windows.cmake)
else()
    message(FATAL_ERROR "system not supported")
endif()

if(${ENABLE_TESTS})
    include(${PROJECT_SOURCE_DIR}/CMake_Build_Platforms/Testing.cmake)
endif()

if(${BUILD_DOCS})
    include(${PROJECT_SOURCE_DIR}/CMake_Build_Platforms/Doxygen.cmake)
endif()
