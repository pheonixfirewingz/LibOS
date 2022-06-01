cmake_minimum_required(VERSION 3.20.0)
project(libos CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_CXX_EXTENSIONS ON)

#options
option(USE_SANATIZER ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_COMPILER_NUMBER 0)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_COMPILER_NUMBER 1)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_COMPILER_NUMBER 2)
else()
    message(FATAL_ERROR "compiler not supported")
endif()


if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    include_directories(${WAYLAND_INCLUDE_DIR})
    include_directories(${PROJECT_SOURCE_DIR}/vcpkg_installed/x64-linux/include)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-linux/share)
    set(CMAKE_SYSTEM_NUMBER 0)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    include_directories(${PROJECT_SOURCE_DIR}/vcpkg_installed/x64-windows/include)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/share)
    set(CMAKE_SYSTEM_NUMBER 1)
endif()
configure_file(src/lib/Cmake.h.in ../src/lib/Cmake.h @ONLY)

# add new packages here
find_package(Vulkan REQUIRED)
find_package(vk-bootstrap CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)

# add non auto includes here

include_directories(${PROJECT_SOURCE_DIR}/Includes)
include_directories(${Vulkan_INCLUDE_DIRS})


set(sources src/lib/FileIO.cpp
    src/lib/RefractileContext.cpp
    src/lib/Graphics/GraphicsContext.cpp
    src/lib/Graphics/CommandBuffer.cpp
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
if(MSVC)
  target_compile_options(libos PRIVATE /W4 /WX)
else()
  target_compile_options(libos PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

if(${USE_SANATIZER})
if(MSVC)
  target_compile_options(libos PRIVATE /fsanitize=address)
else()
  target_compile_options(libos PRIVATE -fsanitize=address,alignment,bounds,null,unreachable,integer -fno-omit-frame-pointer)
endif()
endif()

target_link_libraries(libos PRIVATE Vulkan::Vulkan OpenAL::OpenAL vk-bootstrap::vk-bootstrap)

set(tests_sources src/tests/Main.cpp
    src/tests/FileIOTest.cpp
    src/tests/NetIOTest.cpp)

add_executable(LibOSTests ${tests_sources})
target_link_libraries(LibOSTests libos)