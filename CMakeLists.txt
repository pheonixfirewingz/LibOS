cmake_minimum_required(VERSION 3.16.3)
project(LibOS CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_CXX_EXTENSIONS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/ThirdParty/CmakeModules)
include_directories(${PROJECT_SOURCE_DIR}/Includes)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -fsanitize=address,alignment,bounds,null,unreachable,integer -fno-omit-frame-pointer -Wall -Wextra -Werror")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -Wall -Wextra -Werror")
endif()

set(sources src/lib/FileIO.cpp)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
set(sources ${sources}
src/lib/Linux/FileIO.cpp
src/lib/Linux/NetIO.cpp
src/lib/Linux/Window.cpp
src/lib/Linux/Panic.cpp)
endif()

add_library(LibOS ${sources})

set(tests_sources 	src/tests/Main.cpp 
					src/tests/FileIOTest.cpp
					src/tests/NetIOTest.cpp)

add_executable(LibOSTests ${tests_sources})
#add_executable(ClientTests src/tests/server.cpp)
#add_executable(UDPClientTests src/tests/udpServer.cpp)


target_link_libraries(LibOSTests LibOS)

