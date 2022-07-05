#include "FileIOTest.h"
#include "NetIOTest.h"
#include "Cmake.h"
#include <RefractileAPI.h> 
#include <CrystalOS.h>
#include <cstdio>
#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#define TEST(func)                                              \
    if ((res = func) != LOS_SUCCESS)                            \
    {                                                           \
        printf("%s: %x\n", "the tests failed with ERROR", res); \
        exit(1);                                                \
    }

refHandle handle;

int main()
{
    using namespace std::chrono_literals;
    losResult res;
    TEST(testFileIOMain())
    //TEST(testNetIOMain())

    std::string file CMAKE_SOURCE_DIR;
    file += "/tests";
    TEST(losSetAssetPath(file.c_str()));
    printf("final test  window test\n");

    losWindowInfo info;
    info.sub_window = false;
    std::string name = "this is the test window";
    info.title = name.data();
    info.title_size = name.size();
    info.window_size.width = 720;
    info.window_size.height = 1280;
    info.request_callback = [](const char *object) {
        if (std::string(object) == "refHandle")
            return (void *)handle;

        return (void *)nullptr;
    };
    losWindow window;
    refCommandBuffer buffer;
    refShaderProgram program;
    TEST(losCreateWindow(&window, info));
    TEST(losCreateKeyboard(window));
    TEST(losCreateMouse(window));
    TEST(refCreateRefractileContext(&handle));
    refCreateGraphicContextInfo g_info;
    TEST(refAppendGraphicsContext(handle, window,g_info));
    TEST(refCreateCommandBuffer(handle, &buffer));
    refCreateShaderProgramInfo shader_info;
    shader_info.shader_settings = "$[asset_base]/Shader/settings.json";
    shader_info.vertex_shader = "$[asset_base]/Shader/VertexShader.vert.spirv";
    shader_info.fragment_shader = "$[asset_base]/Shader/fragmentShader.frag.spirv";
    shader_info.pre_compiled = true;

    TEST(refCreateShaderProgram(handle, &program, shader_info));
    #ifdef __linux__
    std::this_thread::sleep_for(2s);
    #else
        while (losUpdateWindow(window) != LOS_WINDOW_CLOSE)
        {
            if (losIsKeyDown(window, LOS_KEYBOARD_X))
                losRequestClose(window);
            TEST(refBeginCommands(handle, buffer));
            const float32 colour[4]{1.0f,1.0f,0.0f,0.0f};
            TEST(refCmdBeginDrawing(handle, buffer,nullptr,colour));
            TEST(refCmdBindShaderProgram(handle, buffer, program));
            TEST(refCmdDraw(handle, buffer,3,1,0,0));
            TEST(refCmdEndDrawing(handle, buffer));
            TEST(refEndCommands(handle, buffer));
            TEST(refExecuteCommands(handle, buffer, true));
        }
    #endif

    TEST(refDestroyCommandBuffer(handle, buffer));
    TEST(refUnAppendGraphicsContext(handle));
    printf("TEST AUDIO API? - [Y/n]:");
    std::string input;
    std::getline (std::cin, input);
    if(input == "Y")
    {
        TEST(refAppendAudioContext(handle));
        refAudioBuffer a_buffer;
        refAudioPlayer player;
        TEST(refCreateAudioBuffer(&a_buffer, "$[asset_base]/Audio/piano2.wav"));
        TEST(refPrepPlayer(&player));
        TEST(refPlay(player, a_buffer, 0, 0, 0, 1));
        std::this_thread::sleep_for(2s);
        TEST(refPause(player));
        std::this_thread::sleep_for(2s);
        TEST(refResume(player));
        std::this_thread::sleep_for(2s);
        TEST(refStop(player));
        TEST(refDestroyAudioBuffer(a_buffer));
        TEST(refUnAppendAudioContext(handle));
        printf("TEST AUDIO COMPLETE\n");
    }
    TEST(refDestroyRefractileContext(handle));
    TEST(losDestroyKeyboard(window));
    TEST(losDestroyMouse(window));
    TEST(losDestroyWindow(window));

    return 0;
}