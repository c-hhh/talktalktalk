#include <WinSock2.h>
#include <process.h>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <unordered_map>
#include <string>
#include "Client.h"


using namespace std;

SOCKET serverSocket = INVALID_SOCKET;		 //������׽���
SOCKADDR_IN g_ClientAddr = { 0 };			 //�ͻ��˵�ַ
int g_iClientAddrLen = sizeof(g_ClientAddr);
string s;

//�ͻ�����Ϣ�ṹ��

unordered_map<string, Client> clients;

unsigned int k = 0;

int StartServer();
void decodeMessage(char recvTemp[], string userName);
void login(const char name[]);
void logout(const char name[]);


int main() {
    StartServer(); //����������
    return 0;
}


//��������
unsigned __stdcall ThreadRecvSend(void* param) {
    string userName = s;
    char recvTemp[128] = {0};
    memset(recvTemp,128,0);
    while (true) {
        memset(recvTemp,128,0);
        int recvNum = recv(clients[userName].sClient, recvTemp, sizeof(recvTemp), 0);
        if(recvNum == -1) {
            cout << userName << "�Ͽ�����" << endl;
            logout(userName.c_str());
            clients.erase(userName);
            break;
        }
        printf("�յ����ģ�%s\n", recvTemp);
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
        printf("���ӳɹ�������\t");
        printf("�û�IP��%s\n", inet_ntoa(g_ClientAddr.sin_addr));
        send(clientSocket,"conn suss", 128, 0);
        char userName[128] = {0};
        recv(clientSocket, userName, sizeof(userName), 0);
        printf("�����%d\t�û�����%s\n", k,userName);
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


//����������
int StartServer() {
    WSADATA wsaData = { 0 };
    SOCKADDR_IN ServerAddr = { 0 };				//����˵�ַ
    USHORT uPort = 4567;						//�����������˿�

    //��ʼ���׽���
    if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
        printf("WSAStartup failed with error code: %d\n", WSAGetLastError());
        return -1;
    }
    //�жϰ汾
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        printf("wVersion was not 2.2\n");
        return -1;
    }
    //�����׽���
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        printf("socket failed with error code: %d\n", WSAGetLastError());
        return -1;
    }

    //���÷�������ַ
    ServerAddr.sin_family = AF_INET;//���ӷ�ʽ
    ServerAddr.sin_port = htons(uPort);//�����������˿�
    ServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//�κοͻ��˶����������������

    //�󶨷�����
    if (SOCKET_ERROR == bind(serverSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr))) {
        printf("bind failed with error code: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        return -1;
    }
    //���ü����ͻ���������
    if (SOCKET_ERROR == listen(serverSocket, 20000)) {
        printf("listen failed with error code: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }
    return waitConn();
}