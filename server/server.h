//
// Created by angry on 2021/3/25.
//
#pragma once
#ifndef LIVEPUSH_SERVER_H
#define LIVEPUSH_SERVER_H

#include <list>
#include <vector>
#include "../data/data.h"
#include <iostream>
#include <thread>

class Server
{
public:
    std::list<RoomInfo *> roomList;         //房间配置信息列表
    std::list<std::string> sendKeyList;     //密钥信息列表
private:
    std::list<std::pair<RoomInfo *, int>> messageList;    //消息队列
    std::vector<std::thread *> threadPool;                    //线程池
public:
    Server(int threadPoolSize);

    void serverRun();       //程序执行入口
private:
    void httpRequest();     //主线程在此循环
    void livePush();        //子线程在此循环
    void roomHandle();      //房间处理
    static void *livePushEnter(void* arg);
};


#endif //LIVEPUSH_SERVER_H
