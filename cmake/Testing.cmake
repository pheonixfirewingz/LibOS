# this script is a mess and will need rewriting
include(FetchContent)
include(GoogleTest)
FetchContent_Declare(
googletest
URL https://github.com/google/googletest/archive/58d77fa8070e8cec2dc1ed015d66b454c8d78850.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
if(CMAKE_SYSTEM_NAME MATCHES Android)
else()
    add_executable(Server src/test/Server.cpp)
    target_link_libraries(Server libos)
    target_include_directories(Server PRIVATE ${PROJECT_SOURCE_DIR}/Includes)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_link_options(Server PRIVATE -static-libgcc -static-libstdc++)
    endif()
    if(MSVC)
        target_compile_options(Server PRIVATE /EHsc)
    endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES WindowsStore)
    set(ASSET_FILES
    tests/Assets/LockScreenLogo.scale-200.png
    tests/Assets/SplashScreen.scale-200.png
    tests/Assets/Square44x44Logo.scale-200.png
    tests/Assets/Square44x44Logo.targetsize-24_altform-unplated.png
    tests/Assets/Square150x150Logo.scale-200.png
    tests/Assets/StoreLogo.png
    tests/Assets/Wide310x150Logo.scale-200.png)

    set(CONTENT_FILES src/test/Package.appxmanifest)
    add_executable(Tests src/test/TestMain.cpp ${ASSET_FILES} ${CONTENT_FILES})
    target_compile_options(Tests PRIVATE /EHsc)
    target_link_libraries(Tests GTest::gtest_main libos)
    target_include_directories(Tests PRIVATE ${PROJECT_SOURCE_DIR}/Includes)
    set_target_properties(Tests PROPERTIES VS_WINRT_COMPONENT TRUE OUTPUT_NAME "Tests"  VS_CPPWINRT true)
    set_property(TARGET Tests PROPERTY VS_PACKAGE_REFERENCES "Microsoft.VCRTForwarders.140")
    set_property(SOURCE ${CONTENT_FILES} PROPERTY VS_DEPLOYMENT_CONTENT 1)
    set_property(SOURCE ${ASSET_FILES} PROPERTY VS_DEPLOYMENT_CONTENT 1)
    set_property(SOURCE ${ASSET_FILES} PROPERTY VS_DEPLOYMENT_LOCATION "Assets")
    gtest_discover_tests(Tests)
elseif(CMAKE_SYSTEM_NAME MATCHES Android)
    add_library(Tests SHARED src/test/AndroidMain.cpp)
    target_link_libraries(Tests libos android game-activity::game-activity jnigraphics ${log-lib})
else()
    add_executable(Tests src/test/TestMain.cpp)
    target_link_libraries(Tests GTest::gtest_main libos)
    if(MSVC)
        target_compile_options(Tests PRIVATE /EHsc)
    endif()
    target_include_directories(Tests PRIVATE ${PROJECT_SOURCE_DIR}/Includes)
    gtest_discover_tests(Tests)
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_options(Tests PRIVATE -static-libgcc -static-libstdc++)
endif()