//
// Created by chhhh on 2021/1/25.
//

#include <string>
#include <unordered_map>
#include "Client.h"

using namespace std;

void store(const char name[], char message[]);
extern unordered_map<string, Client> clients;
const char I = '2';
const char Q = '1';

void sentAllUser(char recvTemp[], string userName){
    string infor;
    for (auto &x:clients) {
        infor = infor + x.first + ",";
    }
    strcpy(recvTemp, "server      ");
    strcat(recvTemp,infor.c_str());
    send(clients[userName].sClient,recvTemp,128,0);
}

void forward(char recvTemp[], string userName){
    string un;
    for(int i = 0; i < 12 && recvTemp[i] != ' '; i++){
        un.push_back(recvTemp[i]);
    }
    for(int i = 0; i < 12; i++){
        if(i < userName.size())
            recvTemp[i] = userName[i];
        else
            recvTemp[i] = ' ';
    }
    if(un == "255255255255") {
        store(userName.c_str(), recvTemp);
        for (auto &x:clients) {
            send(clients[x.first].sClient, recvTemp, 128, 0);
        }
    }
    else {
        if(clients.find(un) == clients.end())
            send(clients[userName].sClient,"server      no user",128,0);
        else
            send(clients[un].sClient, recvTemp, 128, 0);
    }
}


void decodeMessage(char recvTemp[], string userName){
    if(recvTemp[0] == Q){
        sentAllUser(recvTemp + 1, userName);
    }
    else if(recvTemp[0] == I){
        forward(recvTemp + 1, userName);
    }
}