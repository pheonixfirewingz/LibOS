cmake_minimum_required(VERSION 3.16.3)
project(LibOS CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_CXX_EXTENSIONS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/Modules)
include_directories(${PROJECT_SOURCE_DIR}/Includes)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -Wall -Wextra -Werror -lwayland-client")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -Wall -Wextra -Werror -lwayland-client")
endif()

if(${SANITIZERS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,alignment,bounds,null,unreachable,integer -fno-omit-frame-pointer")
endif()

set(sources src/lib/FileIO.cpp)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
set(sources ${sources}
src/lib/Windows/FileIO.cpp
src/lib/Windows/NetIO.cpp
src/lib/Windows/Window.cpp
src/lib/Windows/Panic.cpp
src/lib/Windows/Graphics/GraphicsContext.cpp
src/lib/Windows/Audio/AudioContext.cpp)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
set(sources ${sources}
src/lib/Linux/FileIO.cpp
src/lib/Linux/NetIO.cpp
src/lib/Linux/Window.cpp
src/lib/Linux/Panic.cpp
src/lib/Linux/Graphics/GraphicsContext.cpp
src/lib/Linux/Audio/AudioContext.cpp)
endif()


add_library(LibOS ${sources})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
find_package(Vulkan REQUIRED FATAL-ERROR)
include_directories(${WAYLAND_INCLUDE_DIR})
include_directories(${Vulkan_INCLUDE_DIRS})
target_link_libraries(LibOS ${Vulkan_LIBRARIES})
endif()

set(tests_sources 	src/tests/Main.cpp 
					src/tests/FileIOTest.cpp
					src/tests/NetIOTest.cpp)

add_executable(LibOSTests ${tests_sources})
#add_executable(ClientTests src/tests/server.cpp)
#add_executable(UDPClientTests src/tests/udpServer.cpp)


target_link_libraries(LibOSTests LibOS)

