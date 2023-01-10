set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_CXX_EXTENSIONS ON)
include(FindPkgConfig)
pkg_check_modules(LIBDECOR libdecor-0)
pkg_check_modules(WAYLAND wayland-client)
pkg_check_modules(XKB xkbcommon)
pkg_check_modules(XCB_KEYSYMS xcb-keysyms)
pkg_check_modules(XCB xcb)

set(LibOS_Source ${LibOS_Source}
        # posix
        ${PROJECT_SOURCE_DIR}/src/libOS/NetIO/Posix.cpp
        ${PROJECT_SOURCE_DIR}/src/libOS/FileIO/Posix.cpp
        ${PROJECT_SOURCE_DIR}/src/libOS/Window/Xcb.cpp
        ${PROJECT_SOURCE_DIR}/src/libOS/Window/Wayland.cpp
        ${PROJECT_SOURCE_DIR}/src/libOS/Window/WaylandLibDecor.cpp
        ${PROJECT_SOURCE_DIR}/src/libOS/Window/xdg-shell-protocol.c)

add_library(libos SHARED ${LibOS_Source})
target_include_directories(libos PRIVATE ${PROJECT_SOURCE_DIR}/Includes STATIC)
target_compile_options(libos PRIVATE -W -Werror -Wall -Wextra -Wshadow -Wpedantic -fno-exceptions)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -fno-rtti)
target_link_options(libos PRIVATE -static-libstdc++)
if(${DEBUG_MODE})
  if(${CMAKE_COMPILER_NUMBER} EQUAL 1)
    #target_compile_options(libos PRIVATE -fanalyzer)
  endif()
  target_compile_options(libos PRIVATE -O0 -g3)
else()
  target_compile_options(libos PRIVATE -O3 -g0 -flto -finline-functions)
endif()

target_include_directories(libos PRIVATE ${XCB_INCLUDE_DIR})
target_include_directories(libos PRIVATE ${XCB_KEYSYMS_INCLUDE_DIR})
target_include_directories(libos PRIVATE ${WAYLAND_INCLUDE_DIR})
target_include_directories(libos PRIVATE ${XKB_INCLUDE_DIR})
target_include_directories(libos PRIVATE ${LIBDECOR_INCLUDE_DIRS})
target_include_directories(libos PRIVATE ${PROJECT_SOURCE_DIR}/Includes)
target_include_directories(libos PRIVATE ${PROJECT_SOURCE_DIR}/src/shared STATIC)

target_link_libraries(libos PRIVATE ${LIBDECOR_LIBRARIES} ${XKB_LIBRARIES} ${WAYLAND_LIBRARIES} ${XCB_LIBRARIES} ${XCB_KEYSYMS_LIBRARIES})