#include <chrono>
#include <cstdio>
#include <iostream>
#include <libos/NetIO.h>
#include <string>
#include <thread>
#include <future>
const std::string address = "127.0.0.1";

void udp()
{
    puts("Starting UDP Server");
    losSocket handle;
    losCreateSocketInfo file = {LOS_SOCKET_UDP | LOS_SOCKET_SERVER, address.c_str(), address.size(), 54001};
    if (losCreateSocket(&handle, file) != LOS_SUCCESS)
    {
        printf("UDP Server: failed to create socket\n");
        return;
    }

    bool running = true;

    while (running)
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
        losSocket client;
        losUdpData read_buffer;
        data_size_t read_size = 14;
        losReadSocket(handle, (void *)&read_buffer, read_size);
        printf("UDP Server: %14.14s\n", (char *)read_buffer.data);
        losWriteSocket(handle, (const void *)&read_buffer, read_size);
    }

    if (losDestroySocket(handle) != LOS_SUCCESS)
    {
        printf(" UDP Server: failed to close socket");
        return;
    }
}

void tcp()
{
    puts("Starting TCP Server");
    losSocket handle;
    losCreateSocketInfo file = {LOS_SOCKET_TCP | LOS_SOCKET_SERVER, address.c_str(), address.size(), 54000};
    if (losCreateSocket(&handle, file) != LOS_SUCCESS)
    {
        printf("TCP Server: failed to create socket\n");
        return;
    }

    bool running = true;

    while (running)
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2ms);
        losSocket client;
        if (losWaitForClient(handle, &client) != LOS_SUCCESS)
            exit(-1);
        char read_buffer[14];
        data_size_t read_size = 14;
        losReadSocket(client, (void *)&read_buffer, read_size);
        printf("TCP Server: %14.14s\n", &read_buffer);
        losWriteSocket(client, (const void *)&read_buffer, read_size);
    }

    if (losDestroySocket(handle) != LOS_SUCCESS)
    {
        printf("TCP Server: failed to close socket");
        return;
    }
}

int main()
{
    libOSInit();
    new std::thread([] { udp(); });
    tcp();
    libOSCleanUp();
    return 0;
}