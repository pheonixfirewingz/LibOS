set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_CXX_EXTENSIONS ON)


set(LibOS_Source ${LibOS_Source}
        # winRT
        src/libOS/NetIO/WinRT.cpp
        src/libOS/FileIO/WinRT.cpp
        src/libOS/Window/WinRT.cpp
)

add_library(libos SHARED ${LibOS_Source})
target_include_directories(libos PRIVATE ${PROJECT_SOURCE_DIR}/Includes STATIC)
target_include_directories(libos PRIVATE ${PROJECT_SOURCE_DIR}/src/shared STATIC)
target_compile_options(libos PRIVATE /W4 /WX /await)
set_target_properties(libos PROPERTIES VS_WINRT_COMPONENT TRUE OUTPUT_NAME "libos"  VS_CPPWINRT true)