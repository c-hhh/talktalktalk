//
// Created by chhhh on 2021/1/25.
//
#include <ctime>
#include <iostream>
#include <windows.h>
#include<fstream>
#include<iomanip>

using namespace std;

char fileName[] = "allRecord.txt";
char loginInfor[] = "login.txt";

void login(const char name[]){
    SYSTEMTIME sys;       // 定义一个 SYSTEMTIME 结构体 sys
    GetLocalTime(&sys);   // GetLocalTime是一个Windows API 函数，用来获取当地的当前系统日期和时间。

    ofstream writeFile;            //实例一个写文件对象
    writeFile.open(loginInfor, ios::app);     //创建一个Test.txt文本，并且打开Test.txt文件
    writeFile << sys.wYear << "/" << sys.wMonth << "/" << sys.wDay << "      " << std::setfill('0') << std::setw(2) << sys.wHour
              << ":" << std::setfill('0') << std::setw(2) << sys.wMinute << ":" << std::setfill('0') << std::setw(2) << sys.wSecond << "      ";
    writeFile << name  << "      " << "login" << endl;
}

void logout(const char name[]){
    SYSTEMTIME sys;       // 定义一个 SYSTEMTIME 结构体 sys
    GetLocalTime(&sys);   // GetLocalTime是一个Windows API 函数，用来获取当地的当前系统日期和时间。

    ofstream writeFile;            //实例一个写文件对象
    writeFile.open(loginInfor, ios::app);     //创建一个Test.txt文本，并且打开Test.txt文件
    writeFile << sys.wYear << "/" << sys.wMonth << "/" << sys.wDay << "      " << std::setfill('0') << std::setw(2) << sys.wHour
              << ":" << std::setfill('0') << std::setw(2) << sys.wMinute << ":" << std::setfill('0') << std::setw(2) << sys.wSecond << "      ";
    writeFile << name  << "      " << "logout" << endl;
}

void store(const char name[], char message[]){
    SYSTEMTIME sys;       // 定义一个 SYSTEMTIME 结构体 sys
    GetLocalTime(&sys);   // GetLocalTime是一个Windows API 函数，用来获取当地的当前系统日期和时间。

    ofstream writeFile;            //实例一个写文件对象
    writeFile.open(fileName, ios::app);     //创建一个Test.txt文本，并且打开Test.txt文件
    writeFile << sys.wYear << "/" << sys.wMonth << "/" << sys.wDay << "      " << std::setfill('0') << std::setw(2) << sys.wHour
              << ":" << std::setfill('0') << std::setw(2) << sys.wMinute << ":" << std::setfill('0') << std::setw(2) << sys.wSecond << "      ";
    writeFile << name  << "      " << message << endl;
}

