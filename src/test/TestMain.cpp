#include <codecvt>
#include <gtest/gtest.h>
#include <libos/Defines.h>
#include <libos/FileIO.h>
#include <libos/NetIO.h>
#include <libos/Window.h>
#include <libos/Error.h>
#include <string>
struct testBINARY
{
    uint16_t index = 0;
    size_t name_size;
    const char *name;
};

std::wstring u8convert(const char *utf8Str);
const std::string path = "$[binary_base]\\test.txt";
const std::string path_2 = "$[binary_base]/test.txt";
const std::string path_3 = "$[binary_base]/test.bin";
const std::string path_4 = "libos";
const std::string test_str = "this is a test";
const std::wstring test_str_u = u8convert("this is a test 🫂");
const testBINARY test_struct = {4259, 15, "this is a test"};
const std::string address = "127.0.0.1";
std::string path_one = "$[asset_base]/tests/Shader/VertexShader.vert.spirv";
std::string path_two = "$[asset_base]/tests/Shader/fragmentShader.frag.spirv";
const bool debug_mode = true;

std::wstring u8convert(const char *utf8Str)
{
    std::wstring result;
    const char *ptr = utf8Str;
    while (*ptr != '\0')
    {
        // Determine the number of bytes in the current UTF-8 character
        int numBytes = 0;
        if ((*ptr & 0x80) == 0x00)
        {
            // 1-byte character
            numBytes = 1;
            result += (wchar_t)(*ptr);
        }
        else if ((*ptr & 0xE0) == 0xC0)
        {
            // 2-byte character
            numBytes = 2;
            wchar_t wch = ((*ptr & 0x1F) << 6) | (*(ptr + 1) & 0x3F);
            result += wch;
        }
        else if ((*ptr & 0xF0) == 0xE0)
        {
            // 3-byte character
            numBytes = 3;
            wchar_t wch = ((*ptr & 0x0F) << 12) | ((*(ptr + 1) & 0x3F) << 6) | (*(ptr + 2) & 0x3F);
            result += wch;
        }
        else if ((*ptr & 0xF8) == 0xF0)
        {
            // 4-byte character
            numBytes = 4;
            wchar_t wch =
                ((*ptr & 0x07) << 18) | ((*(ptr + 1) & 0x3F) << 12) | ((*(ptr + 2) & 0x3F) << 6) | (*(ptr + 3) & 0x3F);
            result += wch;
        }
        else
        {
            throw std::runtime_error("Invalid UTF-8 sequence");
        }
        ptr += numBytes;
    }

    return result;
}

TEST(FileIO_Normal, createFileOddPath)
{
    libOSInit();
    losFileHandle handle;
    losFileOpenInfo file{};
    file.fileBits = LOS_FILE_BIT_CREATE;
    file.path = path.data();
    file.path_size = path.size();
    EXPECT_EQ(losDoseFileExist(path.c_str()), LOS_ERROR_MALFORMED_DATA);
    EXPECT_EQ(losOpenFile(&handle, file), LOS_SUCCESS);
    EXPECT_EQ(losDoseFileExist(path.c_str()), LOS_SUCCESS);
    EXPECT_EQ(losCloseFile(handle), LOS_SUCCESS);
    libOSCleanUp();
}

TEST(FileIO_Normal, deleteFileOddPath)
{
    libOSInit();
    losFileHandle handle;
    losFileOpenInfo file{};
    file.fileBits = LOS_FILE_BIT_DELETE_AFTER_CLOSE;
    file.path = path.data();
    file.path_size = path.size();
    EXPECT_EQ(losOpenFile(&handle, file), LOS_SUCCESS);
    EXPECT_EQ(losCloseFile(handle), LOS_SUCCESS);
    libOSCleanUp();
}

TEST(FileIO_Normal, createFile)
{
    libOSInit();
    losFileHandle handle;
    losFileOpenInfo file{};
    file.fileBits = LOS_FILE_BIT_CREATE;
    file.path = path_2.data();
    file.path_size = path_2.size();
    EXPECT_EQ(losOpenFile(&handle, file), LOS_SUCCESS);
    EXPECT_EQ(losCloseFile(handle), LOS_SUCCESS);
    libOSCleanUp();
}

TEST(FileIO_Normal, writeFile)
{
    libOSInit();
    losFileHandle handle;
    losFileOpenInfo file{};
    file.fileBits = LOS_FILE_BIT_WRITE;
    file.path = path_2.data();
    file.path_size = path_2.size();
    EXPECT_EQ(losOpenFile(&handle, file), LOS_SUCCESS);
    EXPECT_EQ(losWriteFile(handle, (void *)test_str.c_str(), test_str.size()), LOS_SUCCESS);
    EXPECT_EQ(losCloseFile(handle), LOS_SUCCESS);
    libOSCleanUp();
}

TEST(FileIO_Normal, readFile)
{
    libOSInit();
    losFileHandle handle;
    losFileOpenInfo file{};
    file.fileBits = LOS_FILE_BIT_READ;
    file.path = path_2.data();
    file.path_size = path_2.size();

    void *read_str = nullptr;
    size_t read_str_size = 0;

    EXPECT_EQ(losOpenFile(&handle, file), LOS_SUCCESS);
    EXPECT_EQ(losReadFile(handle, &read_str, &read_str_size), LOS_SUCCESS);
    EXPECT_GT(read_str_size, 0);
    if (read_str_size > 0)
    {
        std::string read_test((char *)read_str, 0, read_str_size);
        EXPECT_STREQ(read_test.c_str(), test_str.c_str());
    }
    EXPECT_EQ(losCloseFile(handle), LOS_SUCCESS);
    libOSCleanUp();
}

TEST(FileIO_Normal, deleteFile)
{
    libOSInit();
    losFileHandle handle;
    losFileOpenInfo file{};
    file.fileBits = LOS_FILE_BIT_DELETE_AFTER_CLOSE;
    file.path = path_2.data();
    file.path_size = path_2.size();
    EXPECT_EQ(losOpenFile(&handle, file), LOS_SUCCESS);
    EXPECT_EQ(losCloseFile(handle), LOS_SUCCESS);
    libOSCleanUp();
}

TEST(FileIO_Normal_Unicode, createFile)
{
    libOSInit();
    losFileHandle handle;
    losFileOpenInfo file{};
    file.fileBits = LOS_FILE_BIT_CREATE;
    file.path = path_2.data();
    file.path_size = path_2.size();
    EXPECT_EQ(losOpenFile(&handle, file), LOS_SUCCESS);
    EXPECT_EQ(losCloseFile(handle), LOS_SUCCESS);
    libOSCleanUp();
}

TEST(FileIO_Normal_Unicode, writeFile)
{
    libOSInit();
    losFileHandle handle;
    losFileOpenInfo file{};
    file.fileBits = LOS_FILE_BIT_WRITE | LOS_FILE_BIT_UNICODE;
    file.path = path_2.data();
    file.path_size = path_2.size();
    EXPECT_EQ(losOpenFile(&handle, file), LOS_SUCCESS);
    EXPECT_EQ(losWriteFile(handle, (const void *)test_str_u.c_str(), test_str_u.size()), LOS_SUCCESS);
    EXPECT_EQ(losCloseFile(handle), LOS_SUCCESS);
    libOSCleanUp();
}

TEST(FileIO_Normal_Unicode, readFile)
{
    libOSInit();
    losFileHandle handle;
    losFileOpenInfo file{};
    file.fileBits = LOS_FILE_BIT_READ | LOS_FILE_BIT_UNICODE;
    file.path = path_2.data();
    file.path_size = path_2.size();

    void *read_str = nullptr;
    size_t read_str_size = 0;

    EXPECT_EQ(losOpenFile(&handle, file), LOS_SUCCESS);
    EXPECT_EQ(losReadFile(handle, &read_str, &read_str_size), LOS_SUCCESS);
    EXPECT_GT(read_str_size, 0);
    if (read_str_size > 0)
    {
        std::wstring read_test((wchar_t *)read_str, 0, read_str_size);
        EXPECT_LE(read_test.compare(test_str_u), 0);
    }
    EXPECT_EQ(losCloseFile(handle), LOS_SUCCESS);
    libOSCleanUp();
}

TEST(FileIO_Normal_Unicode, deleteFile)
{
    libOSInit();
    losFileHandle handle;
    losFileOpenInfo file{};
    file.fileBits = LOS_FILE_BIT_DELETE_AFTER_CLOSE;
    file.path = path_2.data();
    file.path_size = path_2.size();
    EXPECT_EQ(losOpenFile(&handle, file), LOS_SUCCESS);
    EXPECT_EQ(losCloseFile(handle), LOS_SUCCESS);
    libOSCleanUp();
}

TEST(FileIO_Binary, createFile)
{
    libOSInit();
    losFileHandle handle;
    losFileOpenInfo file{};
    file.fileBits = LOS_FILE_BIT_CREATE;
    file.path = path_3.data();
    file.path_size = path_3.size();
    EXPECT_EQ(losOpenFile(&handle, file), LOS_SUCCESS);
    EXPECT_EQ(losCloseFile(handle), LOS_SUCCESS);
    libOSCleanUp();
}

TEST(FileIO_Binary, writeFile)
{
    libOSInit();
    losFileHandle handle;
    losFileOpenInfo file{};
    file.fileBits = LOS_FILE_BIT_WRITE;
    file.path = path_3.data();
    file.path_size = path_3.size();
    EXPECT_EQ(losOpenFile(&handle, file), LOS_SUCCESS);
    EXPECT_EQ(losWriteFile(handle, &test_struct, sizeof(test_struct)), LOS_SUCCESS);
    EXPECT_EQ(losCloseFile(handle), LOS_SUCCESS);
    libOSCleanUp();
}

TEST(FileIO_Binary, readFile)
{
    libOSInit();
    losFileHandle handle;
    losFileOpenInfo file{};
    file.fileBits = LOS_FILE_BIT_READ;
    file.path = path_3.data();
    file.path_size = path_3.size();

    void *read_str = nullptr;
    size_t read_str_size = 0;

    EXPECT_EQ(losOpenFile(&handle, file), LOS_SUCCESS);
    EXPECT_EQ(losReadFile(handle, &read_str, &read_str_size), LOS_SUCCESS);
    EXPECT_NE(read_str_size, 0);
    if (read_str_size > 0)
    {
        testBINARY *test_read_struct = (testBINARY *)read_str;
        EXPECT_EQ(test_read_struct->index, 4259);
        EXPECT_EQ(test_read_struct->name_size, 15);
        EXPECT_STREQ(test_read_struct->name, test_struct.name);
    }
    EXPECT_EQ(losCloseFile(handle), LOS_SUCCESS);
    libOSCleanUp();
}

TEST(FileIO_Binary, deleteFile)
{
    libOSInit();
    losFileHandle handle;
    losFileOpenInfo file{};
    file.fileBits = LOS_FILE_BIT_DELETE_AFTER_CLOSE;
    file.path = path_3.data();
    file.path_size = path_3.size();
    EXPECT_EQ(losOpenFile(&handle, file), LOS_SUCCESS);
    EXPECT_EQ(losCloseFile(handle), LOS_SUCCESS);
    libOSCleanUp();
}

TEST(FileIO_Lib, dynamicLoad)
{
    libOSInit();
    losFileHandle handle;
    losFileOpenInfo file{};
    file.fileBits = LOS_FILE_BIT_BINARY;
    file.path = path_4.data();
    file.path_size = path_4.size();
    EXPECT_EQ(losOpenFile(&handle, file), LOS_SUCCESS);
    typedef int (*MyFunction)();
#if IS_MSVC
#    pragma warning(push)
#    pragma warning(disable : 4459)
#endif
    MyFunction* address_ = (MyFunction *)losGetFuncAddress(handle, "losWriteFile");
#if IS_MSVC
#    pragma warning(pop)
#endif
    EXPECT_NE(address_, nullptr);
    EXPECT_EQ(losCloseFile(handle), LOS_SUCCESS);
    libOSCleanUp();
}
#ifdef BUILD_EXTENTIONS
#include <libos/extensions/Config.h>
TEST(FileIO_EXTEND_Config_Normal, writeFile)
{
    libOSInit();
    losFileHandle handle;
    EXPECT_EQ(loseGetConfig(&handle,"libos_test_app"), LOS_SUCCESS);
    EXPECT_EQ(losWriteFile(handle, (void *)test_str.c_str(), test_str.size()), LOS_SUCCESS);
    EXPECT_EQ(losCloseFile(handle), LOS_SUCCESS);
    libOSCleanUp();
}

TEST(FileIO_EXTEND_Config_Normal, readFile)
{
    libOSInit();
    losFileHandle handle;

    void *read_str = nullptr;
    size_t read_str_size = 0;

    EXPECT_EQ(loseGetConfig(&handle,"libos_test_app"), LOS_SUCCESS);
    EXPECT_EQ(losReadFile(handle, &read_str, &read_str_size), LOS_SUCCESS);
    EXPECT_GT(read_str_size, 0);
    if (read_str_size > 0)
    {
        std::string read_test((char *)read_str, 0, read_str_size);
        EXPECT_STREQ(read_test.c_str(), test_str.c_str());
    }
    EXPECT_EQ(losCloseFile(handle), LOS_SUCCESS);
    libOSCleanUp();
}
#endif
#ifndef TERMIANL_MODE
TEST(Graphics, Window)
{
    libOSInit();
    losWindow window;
    losWindowInfo info{};
    info.title = test_str.c_str();
    info.title_size = test_str.size();
    info.window_size = {(int64_t)500, (int64_t)500};
    EXPECT_EQ(losCreateWindow(&window, info), LOS_SUCCESS);

    while (losUpdateWindow(window) != LOS_WINDOW_CLOSE)
    {
        using namespace std::chrono;
        using namespace std::literals;
        system_clock::time_point start_time = system_clock::now();
        if (losIsKeyDown(window, LOS_KEY_X))
            losRequestClose(window);
        if (losIsKeyDown(window, LOS_KEY_SPACE))
            losPrintInfo("beep beep!");
        if (losIsMouseDown(window, LOS_LEFT_BUTTON))
        {
            const losSize pos = losRequestMousePosition(window);
#    ifdef ON_WINDOWS
            printf("Mouse Click At -> X: '%I64u ,Y: '%I64u\n", pos.length_one, pos.length_two);
#    else
            printf("Mouse Click At -> X: '%lu ,Y: '%lu\n", pos.length_one, pos.length_two);
#    endif
        }
        system_clock::time_point end_time = system_clock::now();
        std::this_thread::sleep_for(16.5ms - (end_time - start_time));
    }
    losDestroyWindow(window);
    libOSCleanUp();
}
#endif

TEST(NetIO_Client, TCP)
{
    libOSInit();
    losSocket handle;
    losCreateSocketInfo file{};
    file.socket_bits = LOS_SOCKET_TCP;
    file.address = address.c_str();
    file.port = 54000;
    EXPECT_EQ(losCreateSocket(&handle, file), LOS_SUCCESS);
    EXPECT_EQ(losWriteSocket(handle, (const void *)test_str.c_str(), test_str.size()), LOS_SUCCESS);
    char data[14];
    size_t data_size = 14;
    EXPECT_EQ(losReadSocket(handle, (void *)&data, data_size), LOS_SUCCESS);
    EXPECT_STREQ(std::string(data, 0, data_size).c_str(), test_str.c_str());
    EXPECT_EQ(losDestroySocket(handle), LOS_SUCCESS);
    libOSCleanUp();
}

TEST(NetIO_Client, UDP)
{
    libOSInit();
    losSocket handle;
    losCreateSocketInfo file{};
    file.socket_bits = LOS_SOCKET_UDP;
    file.address = address.c_str();
    file.port = 54001;
    const size_t data_size = 14;
    EXPECT_EQ(losCreateSocket(&handle, file), LOS_SUCCESS);
    EXPECT_EQ(losWriteSocket(handle, (const void *)test_str.c_str(), data_size), LOS_SUCCESS);
    char data[data_size];
    EXPECT_EQ(losReadSocket(handle, (void *)&data, data_size), LOS_SUCCESS);
    EXPECT_STREQ(std::string(data, 0, data_size).c_str(), test_str.c_str());
    EXPECT_EQ(losDestroySocket(handle), LOS_SUCCESS);
    libOSCleanUp();
}