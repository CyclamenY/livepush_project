//
// Created by angry on 2021/3/25.
//
#pragma once
#ifndef LIVEPUSH_SERVER_H
#define LIVEPUSH_SERVER_H

#include <list>
#include <curl/curl.h>
#include <vector>
#include "../data/data.h"
#include <iostream>
#include <thread>
#include <unordered_map>
#include <map>
#include <mutex>
#include <condition_variable>
#include "../include/include.h"

class Server
{
public:
    std::list<RoomInfo *> roomList;         //房间配置信息列表
    std::list<std::string> sendKeyList;     //密钥信息列表
private:
    std::thread *pushThread;                          //推送消息线程
    std::list<LiveInfo> messageList;                  //消息队列
//    std::vector<CURL *> roomCurlVec;                     //room curl数组
    std::unordered_map<CURL *, RoomInfo *> roomMap;     //room curl关联roominfo map
    std::map<std::string, CURL *> pushCurlMap;                     //推送curl map
    std::mutex lock;                                    //互斥锁
    std::condition_variable cond;                       //条件变量
public:
    static bool runFlag;                                //运行变量
    void serverRun();       //程序执行入口
//    bool roomCurlVecPush(CURL *curl)
//    {
//        roomCurlVec.emplace_back(curl);
//        return true;
//    };

    bool pushCurlMapPush(const std::string &sendKey, CURL *curl)
    {
        pushCurlMap[sendKey] = curl;
        return true;
    };

    bool roomCurlMapPush(CURL *curl, RoomInfo *room)
    {
        roomMap[curl] = room;
        return true;
    };

    ~Server()
    {
        for (auto const &it:roomList)
            delete it;
        delete pushThread;
    }

private:
    void httpRequest();     //主线程在此循环
    void livePush();        //子线程在此循环
};


#endif //LIVEPUSH_SERVER_H
