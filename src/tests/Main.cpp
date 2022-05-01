#include "FileIOTest.h"
#include "NetIOTest.h"
#include <CrystalOS.h>
#include <string>
#define TEST(func)                                                \
    if ((res = func) != LOS_SUCCESS)                              \
    {                                                             \
        os_print("%s: %x\n", "the tests failed with ERROR", res); \
    }

int main()
{
    losResult res;
    TEST(testFileIOMain())
    TEST(testNetIOMain())
    os_print("final test  window test");
    losWindowInfo info;
    info.subWindow = false;
    std::string name = "this is the test window";
    info.title = name.data();
    info.title_size = name.size();
    info.windowSize = {720, 1280};
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