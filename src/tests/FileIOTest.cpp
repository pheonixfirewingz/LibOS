#include "FileIOTest.h"
#include <string>
#define TEST(File_Bits, Path, Test_Func)                       \
    {                                                          \
        losFileOpenInfo file;                                  \
        file.fileBits = File_Bits;                             \
        file.path = Path;                                      \
        file.path_size = sizeof(Path);                         \
        if ((res = losOpenFile(&handle, file)) != LOS_SUCCESS) \
            return res;                                        \
        if ((res = Test_Func(handle)) != LOS_SUCCESS)          \
            return res;                                        \
        if ((res = losCloseFile(handle)) != LOS_SUCCESS)       \
            return res;                                        \
    }

losResult testNNorm(losFileHandle)
{
    return LOS_SUCCESS;
}

losResult testNWrite(losFileHandle handle)
{
    std::string test_write_str = "this is a test";
    return losWriteFile(handle, (void *)test_write_str.c_str(), test_write_str.size());
}

losResult testNRead(losFileHandle handle)
{
    losResult res;
    void *read_str;
    size read_str_size = 0;
    if ((res = losReadFile(handle, &read_str, &read_str_size)) != LOS_SUCCESS)
        return res;

    std::string test_read_str((char*)read_str, 0, read_str_size);

    if (read_str_size == 0)
        return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
    
    if (test_read_str != "this is a test")
        return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;

    return res;
}

struct testBINARY
{
    uint16 index = 0;
    size name_size;
    std::string name;
};

losResult testBWrite(losFileHandle handle)
{
    testBINARY test_write_str;
    test_write_str.index = 4259;
    test_write_str.name_size = 15;
    test_write_str.name = "this is a test";

    return losWriteFile(handle, (void *)&test_write_str, sizeof(testBINARY));
}

losResult testBRead(losFileHandle handle)
{
    losResult res;
    testBINARY test_write_str;
    test_write_str.index = 4259;
    test_write_str.name_size = 15;
    test_write_str.name = "this is a test";

    void *read_str;
    size read_str_size = 0;
    if ((res = losReadFile(handle, &read_str, &read_str_size)) != LOS_SUCCESS)
        return res;

    if (read_str_size == 0)
        return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;

    testBINARY* test_read_str = (testBINARY *)read_str;

    if (test_read_str->index != test_write_str.index)
        return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;

     if (test_read_str->name_size != test_write_str.name_size)
        return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;

    if (test_read_str->name != test_write_str.name)
        return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;

    return res;
}

// TODO: add tests for LOS_FILE_BIT_FLIP_ENDIEN
losResult testFileIOMain()
{
    printf("FILEIO TESTS:\n");
    losResult res;
    losFileHandle handle;
    std::string path = "$[binary_base]\\test.txt";
    std::string path_2 = "$[binary_base]/test.txt";
    std::string path_3 = "$[binary_base]/test.bin";

    printf("create file odd path test(Normal)\n");
    TEST(LOS_FILE_BIT_CREATE, path.data(), testNNorm)
    printf("delete file odd path test (Normal)\n");
    TEST(LOS_FILE_BIT_DELETE_AFTER_CLOSE, path.data(), testNNorm)
    printf("create file test (Normal)\n");
    TEST(LOS_FILE_BIT_CREATE, path_2.data(), testNNorm)
    printf("write file test (Normal)\n");
    TEST(LOS_FILE_BIT_WRITE, path_2.data(), testNWrite)
    printf("read file test (Normal)\n");
    TEST(LOS_FILE_BIT_READ, path_2.data(), testNRead)
    printf("delete file test (Normal)\n");
    TEST(LOS_FILE_BIT_DELETE_AFTER_CLOSE, path_2.data(), testNNorm)
    printf("create file test (BINARY)\n");
    TEST(LOS_FILE_BIT_CREATE | LOS_FILE_BIT_BINARY, path_3.data(), testNNorm)
    printf("write file test (BINARY)\n");
    TEST(LOS_FILE_BIT_WRITE | LOS_FILE_BIT_BINARY, path_3.data(), testBWrite)
    printf("read file test (BINARY)\n");
    TEST(LOS_FILE_BIT_READ | LOS_FILE_BIT_BINARY, path_3.data(), testBRead)
    printf("delete file test (BINARY)\n");
    TEST(LOS_FILE_BIT_DELETE_AFTER_CLOSE | LOS_FILE_BIT_BINARY, path_3.data(), testNNorm)
    printf("all tests passed\n");
    printf("\n");
    return LOS_SUCCESS;
}
