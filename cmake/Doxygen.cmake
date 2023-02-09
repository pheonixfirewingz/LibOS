find_package(Doxygen)
message(STATUS "Can build doc?  ${DOXYGEN_FOUND}")
if(DOXYGEN_FOUND)

    set(LibOS_API
    ${PROJECT_SOURCE_DIR}/Includes/libos/FileIO.h
    ${PROJECT_SOURCE_DIR}/Includes/libos/NetIO.h
    ${PROJECT_SOURCE_DIR}/Includes/libos/Window.h
    ${PROJECT_SOURCE_DIR}/Includes/libos/NativeWindow.h
    ${PROJECT_SOURCE_DIR}/Includes/libos/DataType.h
    ${PROJECT_SOURCE_DIR}/Includes/libos/Defines.h)
    
    option(BUILD_DOC "Build documentation" ON)
    set(DOXYGEN_MULTILINE_CPP_IS_BRIEF YES)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_MAN YES)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_DISABLE_INDEX NO)
    set(DOXYGEN_FULL_SIDEBAR NO)
    set(DOXYGEN_HTML_HEADER "${PROJECT_SOURCE_DIR}/src/doxygen-custom/header.html")
    set(DOXYGEN_HTML_EXTRA_FILES "${PROJECT_SOURCE_DIR}/src/doxygen/doxygen-awesome-darkmode-toggle.js")
    set(DOXYGEN_HTML_EXTRA_STYLESHEET "${PROJECT_SOURCE_DIR}/src/doxygen/doxygen-awesome.css" "${PROJECT_SOURCE_DIR}/src/doxygen/doxygen-awesome-sidebar-only.css" "${PROJECT_SOURCE_DIR}/src/doxygen/doxygen-awesome-sidebar-only-darkmode-toggle.css")
    
    doxygen_add_docs(doxygen ${LibOS_API} COMMENT "Generate Docs")

    if(BUILD_DOCS)
        add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.doxygen
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
    endif()
endif()