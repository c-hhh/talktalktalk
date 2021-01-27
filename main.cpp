#include <WinSock2.h>
#include <process.h>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <unordered_map>
#include <string>
#include "Client.h"


using namespace std;

SOCKET serverSocket = INVALID_SOCKET;		 //服务端套接字
SOCKADDR_IN g_ClientAddr = { 0 };			 //客户端地址
int g_iClientAddrLen = sizeof(g_ClientAddr);
string s;

//客户端信息结构体

unordered_map<string, Client> clients;

unsigned int k = 0;

int StartServer();
void decodeMessage(char recvTemp[], string userName);
void login(const char name[]);
void logout(const char name[]);


int main() {
    StartServer(); //启动服务器
    return 0;
}


//接受请求
unsigned __stdcall ThreadRecvSend(void* param) {
    string userName = s;
    char recvTemp[128] = {0};
    memset(recvTemp,128,0);
    while (true) {
        memset(recvTemp,128,0);
        int recvNum = recv(clients[userName].sClient, recvTemp, sizeof(recvTemp), 0);
        if(recvNum == -1) {
            cout << userName << "断开连接" << endl;
            logout(userName.c_str());
            clients.erase(userName);
            break;
        }
        printf("收到报文：%s\n", recvTemp);
        decodeMessage(recvTemp, userName);
    }
    return 0;
}

int waitConn(){
    while (true) {
        SOCKET clientSocket = INVALID_SOCKET;
        if ((clientSocket = accept(serverSocket, (SOCKADDR*)&g_ClientAddr, &g_iClientAddrLen)) == INVALID_SOCKET) {
            printf("accept failed with error code: %d\n", WSAGetLastError());
            closesocket(serverSocket);
            WSACleanup();
            return -1;
        }
        printf("连接成功。。。\t");
        printf("用户IP是%s\n", inet_ntoa(g_ClientAddr.sin_addr));
        send(clientSocket,"conn suss", 128, 0);
        char userName[128] = {0};
        recv(clientSocket, userName, sizeof(userName), 0);
        printf("序号是%d\t用户名是%s\n", k,userName);
        login(userName);
        k++;
        Client client;
        s = userName;
        client.sClient = clientSocket;
        strcpy(client.userName,userName);
        strcpy(client.IP,inet_ntoa(g_ClientAddr.sin_addr));
        clients[s] = client;
        _beginthreadex(NULL, 0, ThreadRecvSend, NULL, 0, 0);
    }
    return 0;
}


//启动服务器
int StartServer() {
    WSADATA wsaData = { 0 };
    SOCKADDR_IN ServerAddr = { 0 };				//服务端地址
    USHORT uPort = 4567;						//服务器监听端口

    //初始化套接字
    if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
        printf("WSAStartup failed with error code: %d\n", WSAGetLastError());
        return -1;
    }
    //判断版本
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        printf("wVersion was not 2.2\n");
        return -1;
    }
    //创建套接字
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        printf("socket failed with error code: %d\n", WSAGetLastError());
        return -1;
    }

    //设置服务器地址
    ServerAddr.sin_family = AF_INET;//连接方式
    ServerAddr.sin_port = htons(uPort);//服务器监听端口
    ServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//任何客户端都能连接这个服务器

    //绑定服务器
    if (SOCKET_ERROR == bind(serverSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr))) {
        printf("bind failed with error code: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        return -1;
    }
    //设置监听客户端连接数
    if (SOCKET_ERROR == listen(serverSocket, 20000)) {
        printf("listen failed with error code: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }
    return waitConn();
}