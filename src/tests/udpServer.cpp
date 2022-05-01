/**********************************************************************
    Name		: Example UDP Server
    Author		: Sloan Kelly
    Date		: 2017-12-16
    Purpose		: Example of a bare bones UDP server

***********************************************************************/
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <iostream>

// Saves us from typing std::cout << etc. etc. etc.
using namespace std;

// Main entry point into the server
int main()
{
    ////////////////////////////////////////////////////////////
    // SOCKET CREATION AND BINDING
    ////////////////////////////////////////////////////////////

    // Create a socket, notice that it is a user datagram socket (UDP)
    int in = socket(AF_INET, SOCK_DGRAM, 0);

    // Create a server hint structure for the server
    sockaddr_in serverHint;
    serverHint.sin_addr.s_addr = INADDR_ANY; // Us any IP address available on the machine
    serverHint.sin_family = AF_INET;            // Address format is IPv4
    serverHint.sin_port = htons(54000);         // Convert from little to big endian

    // Try and bind the socket to the IP and port
    if (bind(in, (sockaddr *)&serverHint, sizeof(serverHint)) < 0)
    {
        perror("Can't bind socket!: ");
        return -1;
    }

    ////////////////////////////////////////////////////////////
    // MAIN LOOP SETUP AND ENTRY
    ////////////////////////////////////////////////////////////

    sockaddr_in client;                // Use to hold the client information (port / ip address)
    socklen_t clientLength = sizeof(client); // The size of the client information

    char buf[1024];

    // Enter a loop
    while (true)
    {
        memset(&client,0,clientLength); // Clear the client structure
        memset(buf,0 , 1024);             // Clear the receive buffer

        // Wait for message
        int bytesIn = recvfrom(in, buf, 1024, 0, (sockaddr *)&client, &clientLength);
        if (bytesIn < 0)
        {
            perror("Error receiving from client: ");
            break;
        }

        // Display message and client info
        char clientIp[256];        // Create enough space to convert the address byte array
        memset(clientIp, 0,256); // to string of characters

        // Convert from byte array to chars
        inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

        // Display the message / who sent it
        cout << "Message recv from " << clientIp << " : " << buf << endl;
    }

    // Close socket
    close(in);
}
