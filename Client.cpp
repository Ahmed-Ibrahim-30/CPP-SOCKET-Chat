//
// Created by ahmed Ibrahim on 03-Aug-25.
//

#include "iostream"
#include "winsock2.h"

using namespace std;
#pragma comment (lib , "Ws2_32.lib")

#define PORT 8080

int main()
{
    WSAData wsaData{};
    int res = WSAStartup(MAKEWORD(2,2) , &wsaData);

    if (res)
    {
        cout<<"Client Size Can't initialize WinSock\n";
        return 0;
    }

    //1- Initialize Socket
    SOCKET socket1 = socket(AF_INET , SOCK_STREAM , 0);

    if (socket1 == INVALID_SOCKET)
    {
        cerr << "Socket creation failed.\n";
        WSACleanup();
        return 1;
    }

    //Initialize Socket Address
    sockaddr_in clientSocketAdr{};

    clientSocketAdr.sin_family = AF_INET;
    clientSocketAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientSocketAdr.sin_port = htons(PORT);

    /*
     * Client make Connection request on specific Socket Address
     */
    int connRes = -1;
    while (connRes == -1)
    {
        connRes = connect(socket1 , (SOCKADDR*)& clientSocketAdr , sizeof (clientSocketAdr));
        if (connRes == SOCKET_ERROR)
        {
            cerr<<"Client Binding Error\n";
            continue;
        }
    }

    string message ;
    cout<<"Enter Message\n";
    getline(cin , message);

    send(socket1 , message.c_str() , (int)message.size() , 0);

    char buffer[1024];
    int bytes = recv(socket1 , buffer , sizeof(buffer) , 0);
    buffer[bytes] = '\0';

    cout<<"Server Say : "<<buffer<<"\n";

    closesocket(socket1);
    WSACleanup();
    return 0;
}