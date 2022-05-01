cmake_minimum_required(VERSION 3.16.3)
project(LibOS CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/ThirdParty/CmakeModules)
include_directories(${PROJECT_SOURCE_DIR}/Includes STATIC)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(ERROR "can no use this compiler")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -static-libstdc++ -static-libgcc")
endif()

set(sources src/lib/FileIO.cpp 
					src/lib/NetIO.cpp 
					src/lib/Window.cpp)

add_library(LibOS ${sources})

set(tests_sources 	src/tests/Main.cpp 
					src/tests/FileIOTest.cpp
					src/tests/NetIOTest.cpp)

add_executable(LibOSTests ${tests_sources})
#add_executable(ClientTests src/tests/server.cpp)
#add_executable(UDPClientTests src/tests/udpServer.cpp)


target_link_libraries(LibOSTests LibOS)

