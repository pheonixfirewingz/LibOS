cmake_minimum_required(VERSION 3.22.0)
project(libos CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_CXX_EXTENSIONS ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMake_Modules)

#options
option(USE_SANATIZER ON)
set(BUILD_OF_JNI ON)

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
    find_package(Wayland REQUIRED)
    include_directories(${WAYLAND_INCLUDE_DIR})
    set(CMAKE_SYSTEM_NUMBER 0)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(CMAKE_SYSTEM_NUMBER 1)
endif()

configure_file(src/lib/Cmake.h.in ../src/lib/Cmake.h @ONLY)
configure_file(src/tests/Cmake.h.in ../src/tests/Cmake.h @ONLY)

# add new packages here
find_package(Vulkan REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(FreeALUT CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(unofficial-vulkan-memory-allocator CONFIG REQUIRED)
if(${BUILD_OF_JNI})
  find_package(JNI REQUIRED)
  if(!${JNI_FOUND})
    message( FATAL_ERROR "jni is no available" )
  endif()
endif()

# add non auto includes here
include_directories(${PROJECT_SOURCE_DIR}/Includes)


set(sources src/lib/FileIO.cpp
    src/lib/RefractileContext.cpp

    # Graphics
    src/lib/Graphics/ShaderProgram.cpp
    src/lib/Graphics/GraphicsContext.cpp
    src/lib/Graphics/CommandBuffer.cpp
    src/lib/Graphics/FrameBuffer.cpp

    # Audio
    src/lib/Audio/AudioContext.cpp
    src/lib/Audio/AudioBuffers.cpp

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

if(${BUILD_OF_JNI})
  set(sources ${sources}
  src/lib/Jni/FileIO.cpp
  )
endif()

add_library(libos SHARED ${sources})
target_include_directories(libos PRIVATE ${Vulkan_INCLUDE_DIRS})
if(${BUILD_OF_JNI})
  target_include_directories(libos PRIVATE ${JNI_INCLUDE_DIRS})
endif()

if(MSVC)
  target_compile_options(libos PRIVATE /W4 /WX)
else()
  target_compile_options(libos PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  target_link_options(libos PRIVATE -lwayland-client)
endif()

if(${USE_SANATIZER})
if(MSVC)
  target_compile_options(libos PRIVATE /fsanitize=address)
else()
  target_compile_options(libos PRIVATE -fsanitize=address,alignment,bounds,null,unreachable,integer -fno-omit-frame-pointer)
endif()
endif()

set(LINK_LIBS Vulkan::Vulkan OpenAL::OpenAL FreeALUT::alut JsonCpp::JsonCpp unofficial::vulkan-memory-allocator::vulkan-memory-allocator)
if(${BUILD_OF_JNI})
  set(LINK_LIBS ${LINK_LIBS} ${JNI_LIBRARIES})
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(LINK_LIBS ${LINK_LIBS} ${WAYLAND_LIBRARIES})
endif()

target_link_libraries(libos PRIVATE ${LINK_LIBS}) 

set(tests_sources src/tests/Main.cpp
    src/tests/FileIOTest.cpp
    src/tests/NetIOTest.cpp)

add_executable(LibOSTests ${tests_sources})
target_link_libraries(LibOSTests libos)
