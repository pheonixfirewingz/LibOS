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
    TEST(testFileIOMain())
    TEST(testNetIOMain())
    printf("final test  window test\n");
    losWindowInfo info;
    info.subWindow = false;
    std::string name = "this is the test window";
    info.title = name.data();
    info.title_size = name.size();
    info.windowSize.width = 720;
    info.windowSize.height = 1280;

    losWindow window;
    TEST(losCreateWindow(&window, info));
    TEST(losCreateKeyboard(window));
    TEST(losCreateMouse(window));

    while (losUpdateWindow(window) != LOS_WINDOW_CLOSE)
    {
        if (losIsKeyDown(window, LOS_KEYBOARD_X))
            losRequestClose(window);
    }

    TEST(losDestoryKeyboard(window));
    TEST(losDestoryMouse(window));
    TEST(losDestoryWindow(window));

    return 0;
}