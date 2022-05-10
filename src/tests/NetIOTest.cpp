#include "NetIOTest.h"
#include <string>
#define TEST(File_Bits, Path,Port, Test_Func)                       \
    {                                                          \
        losCreateSocketInfo file;                              \
        file.socketBits = File_Bits;                           \
        file.address = Path;                                   \
        file.port = Port;                                       \
        if ((res = losCreateSocket(&handle, file)) != LOS_SUCCESS) \
            return res;                                        \
        if ((res = Test_Func(handle)) != LOS_SUCCESS)          \
            return res;                                        \
        if ((res = losDestorySocket(handle)) != LOS_SUCCESS)       \
            return res;                                        \
    }

losResult TCPtest(losSocket handle)
{
    losResult ret;
    ret = losWriteSocket(handle, "this is a tests", 16);
    if (ret != LOS_SUCCESS)
        return ret;

    char data[16];
    size data_size = 16;
    ret = losReadSocket(handle, (void*) &data, &data_size);
    if (ret != LOS_SUCCESS)
        return ret;

    printf("Returned:%s\n", std::string(data, 0, data_size).c_str());
    
    return LOS_SUCCESS;
}

losResult UDPtest(losSocket handle)
{
    losResult ret;
    ret = losWriteSocket(handle, "this is a tests", 16);
    if (ret != LOS_SUCCESS)
        return ret;

    return LOS_SUCCESS;
}

losResult testNetIOMain()
{
    printf("NET_IO TESTS:\n");
    losResult res;
    losSocket handle;
    std::string address = "127.0.0.1";
    TEST(LOS_SOCKET_UDP, address.data(), 54000, UDPtest);
    printf("test UDP Client passed\n");
    TEST(LOS_SOCKET_TCP, address.data(), 54000, TCPtest);
    printf("test TCP Client passed\n");
    printf("all tests passed\n");
    printf("\n");
    return LOS_SUCCESS;
}
