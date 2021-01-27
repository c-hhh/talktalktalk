//
// Created by chhhh on 2021/1/25.
//

#ifndef TALKSERVER_CLIENT_H
#define TALKSERVER_CLIENT_H

#include <WinSock2.h>

struct Client {
    SOCKET sClient;      //客户端套接字
    char userName[20];   //客户端用户名
    char IP[20];		 //客户端IP
};

#endif //TALKSERVER_CLIENT_H
