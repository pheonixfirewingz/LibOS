#include "FileIOTest.h"
#include "NetIOTest.h"
#include <CrystalOS.h>
#include <cstdio>
#include <string>
#define TEST(func)                                                \
    if ((res = func) != LOS_SUCCESS)                              \
    {                                                             \
        printf("%s: %x\n", "the tests failed with ERROR", res); \
        exit(1); \
    }

int main()
{
    losResult res;
    //TEST(testFileIOMain())
    //TEST(testNetIOMain())
    printf("final test  window test\n");
    losWindowInfo info;
    info.sub_window = false;
    std::string name = "this is the test window";
    info.title = name.data();
    info.title_size = name.size();
    info.window_size.width = 720;
    info.window_size.height = 1280;

    losWindow window;
    TEST(losCreateWindow(&window, info));
    TEST(losCreateKeyboard(window));
    TEST(losCreateMouse(window));

    while (losUpdateWindow(window) != LOS_WINDOW_CLOSE)
    {
        if (losIsKeyDown(window, LOS_KEYBOARD_X))
            losRequestClose(window);
    }

    TEST(losDestroyKeyboard(window));
    TEST(losDestroyMouse(window));
    TEST(losDestroyWindow(window));

    return 0;
}