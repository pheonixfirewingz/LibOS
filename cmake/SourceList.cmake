function(add_files)
    cmake_parse_arguments(PARAM "" "" "CONDITION" ${ARGN})
    set(PARAM_FILES "${PARAM_UNPARSED_ARGUMENTS}")

    if(PARAM_CONDITION)
        if(NOT (${PARAM_CONDITION}))
            return()
        endif()
    endif()

    foreach(FILE IN LISTS PARAM_FILES)
        target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/${FILE})
    endforeach()
endfunction()