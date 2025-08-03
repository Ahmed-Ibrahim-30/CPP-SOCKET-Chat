//
// Created by ahmed Ibrahim on 03-Aug-25.
//

#include "iostream"
#include "winsock2.h"
#include "thread"
using namespace std;

/**
 * Compiler directive used in Windows C++ projects.
 * It tells the linker to link against the WinSock library (Ws2_32.lib)
 * which contains the implementation of networking functions like:
 * WSAStartup() ---- socket() --- connect()
 */
#pragma comment(lib, "Ws2_32.lib")

#define PORT 8080


void handleClient(const SOCKET &clientSocket)
{
    char buffer[1024];
    int bytes = recv(clientSocket , buffer , sizeof(buffer) , 0);
    buffer[bytes] = '\0';

    cout<<"Client Say --> "<<buffer<<"\n";

    string serverMessage = "Hello From Server\n";
    send(clientSocket , serverMessage.c_str() , (int)serverMessage.size() , 0);
}
int main()

{
    WSAData wsaData{};

    /**
     * Windows-specific line used to initialize the Winsock library
     * MAKEWORD(2, 2):This macro creates a WORD value representing the version of Winsock 2.2
     * If res == 0, it means success.
     * If res != 0, an error occurred
     */
    int res = WSAStartup(MAKEWORD(2,2) , &wsaData);

    if (res != 0)
    {
        cerr<<"Can't Initialize WinSock\n";
        return 0;
    }

    //
    /**
     * Create Socket
     * SOCKET socket1 = socket(af , type , protocol);
     * AF   : This specifies the address family — IP4 (AF_INET) , IP6 (AF_INET6) , Unix domain sockets (not used in Windows) (AF_UNIX)
     * Type : TCP (SOCK_STREAM) ------ UDP (SOCK_DGRAM)
     * Protocol :
     */
    SOCKET socket1 = socket(AF_INET , SOCK_STREAM , 0);

    if (socket1 == INVALID_SOCKET)
    {
        cerr << "Socket creation failed.\n";
        WSACleanup();
        return 1;
    }


    /**
     * Identify Socket Address
     */

    sockaddr_in socketAddress{};
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    socketAddress.sin_port = htons(PORT);

    /**
     * Bind : Associates the socket with a specific IP address and port number.
     */
    int bindRes = bind(socket1 , (SOCKADDR *)&socketAddress , sizeof socketAddress);

    if (bindRes == SOCKET_ERROR)
    {
        cerr<<"Server Binding Error\n";
        return 0;
    }

    /**
     * Tells the OS to start listening for incoming connection requests on the bound socket.
     */

    listen(socket1 , 0);
    SOCKET clientSocket = INVALID_SOCKET;
    do
    {
        clientSocket = accept(socket1 , nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET)break;

        thread clientThread (handleClient , clientSocket);
        clientThread.detach();
    } while (true);

    closesocket(socket1);
    closesocket(clientSocket);
    WSACleanup();
    return 0;


}