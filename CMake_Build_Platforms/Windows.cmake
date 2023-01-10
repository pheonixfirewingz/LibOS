set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_CXX_EXTENSIONS ON)

set(LibOS_Source ${LibOS_Source}
    # windows
    src/libOS/FileIO/Win32.cpp
    src/libOS/NetIO/Win32.cpp
    src/libOS/Window/Win32.cpp)


add_library(libos SHARED ${LibOS_Source})
target_include_directories(libos PRIVATE ${PROJECT_SOURCE_DIR}/Includes STATIC)
target_include_directories(libos PRIVATE ${PROJECT_SOURCE_DIR}/src/shared STATIC)

if(MSVC)
        if(${DEBUG_MODE})
              if(${CMAKE_COMPILER_NUMBER} EQUAL 1)
                    #target_compile_options(libos PRIVATE -fanalyzer)
              endif()
              target_compile_options(libos PRIVATE /Od /Wall /sdl /DEBUG:FULL)
      else()
              target_compile_options(libos PRIVATE /Ot /sdl  /OPT:REF,ICF,LBR /RELEASE)
       endif()
else()
      if(${DEBUG_MODE})
              if(${CMAKE_COMPILER_NUMBER} EQUAL 1)
                    #target_compile_options(libos PRIVATE -fanalyzer)
              endif()
              target_compile_options(libos PRIVATE -O0 -g3)
      else()
              target_compile_options(libos PRIVATE -O3 -g0 -flto -finline-functions)
       endif()
endif()