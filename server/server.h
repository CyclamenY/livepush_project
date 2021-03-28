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
#include <unordered_map>

class Server
{
public:
    std::list<RoomInfo *> roomList;         //房间配置信息列表
    std::list<std::string> sendKeyList;     //密钥信息列表
private:
    std::thread *push;
    std::list<LiveInfo> messageList;                  //消息队列
    std::vector<CURL *> roomCurlVec;                     //room curl数组
    std::unordered_map<CURL *, RoomInfo *> roomMap;     //room curl关联roominfo map
    std::vector<CURL *> pushCurlVec;                     //推送curl数组
public:

    void serverRun();       //程序执行入口
    bool roomCurlVecPush(CURL *curl)
    {
        roomCurlVec.emplace_back(curl);
        return true;
    };

    bool roomCurlMapPush(CURL *curl, RoomInfo *room)
    {
        roomMap[curl] = room;
        return true;
    };
private:
    void httpRequest();     //主线程在此循环
    void livePush();        //子线程在此循环
    void roomHandle();      //房间处理
    static void *livePushEnter(void *arg);
};


#endif //LIVEPUSH_SERVER_H
