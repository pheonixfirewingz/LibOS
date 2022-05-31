cmake_minimum_required(VERSION 3.16.3)
project(libos CXX)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_CXX_EXTENSIONS ON)

include_directories(${PROJECT_SOURCE_DIR}/Includes)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    include_directories(${WAYLAND_INCLUDE_DIR})
    include_directories(${PROJECT_SOURCE_DIR}/vcpkg_installed/x64-linux/include)
    set(CMAKE_SYSTEM_NUMBER 0)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    include_directories(${PROJECT_SOURCE_DIR}/vcpkg_installed/x64-windows/include)
    set(CMAKE_SYSTEM_NUMBER 1)
endif()

configure_file(src/lib/Cmake.h.in ../src/lib/Cmake.h @ONLY)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}-fPIC -m64 -Wall -Wextra -Werror -lwayland-client")

if(${SANITIZERS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,alignment,bounds,null,unreachable,integer -fno-omit-frame-pointer")
endif()

set(sources src/lib/FileIO.cpp
    src/lib/RefractileContext.cpp
    src/lib/Graphics/GraphicsContext.cpp
    src/lib/Audio/AudioContext.cpp

    # windows
    src/lib/Windows/FileIO.cpp
    src/lib/Windows/NetIO.cpp
    src/lib/Windows/Window.cpp
    src/lib/Windows/Panic.cpp

    # posix
    src/lib/Linux/FileIO.cpp
    src/lib/Linux/NetIO.cpp

    # linux
    src/lib/Linux/Window.cpp
    src/lib/Linux/Panic.cpp)

add_library(libos SHARED ${sources})

find_package(Vulkan CONFIG REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})
find_package(OpenAL CONFIG REQUIRED)
include_directories(${OpenAL_INCLUDE_DIRS})
target_link_libraries(libos PRIVATE OpenAL::OpenAL Vulkan::Vulkan)

set(tests_sources src/tests/Main.cpp
    src/tests/FileIOTest.cpp
    src/tests/NetIOTest.cpp)

add_executable(LibOSTests ${tests_sources})
target_link_libraries(LibOSTests libos)