//
// Created by angry on 2021/3/25.
//

#include "data.h"
#include <jsoncpp/json/json.h>
#include <iostream>
#include <gq/Document.h>
#include <gq/Node.h>

size_t RoomInfo::req_reply(void *ptr, size_t size, size_t nmemb, void *stream)
{
    if (stream == nullptr || ptr == nullptr || size == 0)
        return 0;
    size_t realsize = size * nmemb;
    auto *buffer = (std::string *)stream;
    if (buffer != nullptr)
        buffer->append((const char *)ptr, realsize);
    return realsize;
}

LiveInfo BilibiliRoomInfo::roomHandle()
{
    //TODO:业务逻辑
    Json::Reader reader;
    Json::Value root;
    LiveInfo info;
    info.liverName = this->channelName;
    if (reader.parse(response, root))
    {
        if (root["code"].asInt() == 1)
            info.status = ERR;
#ifdef __DEBUG__
        std::cout << this->channelName << " live_status:" << root["data"]["live_status"].asInt() << std::endl;
#endif
        if (root["data"]["live_status"].asInt() == 1)
        {
            info.status = ONLINE;
            info.title = root["data"]["title"].asString();
#ifdef __DEBUG__
            std::cout << info.title << std::endl;
#endif
        }
        else
            info.status = OFFLINE;
    }
    response.clear();
    return info;
}

LiveInfo YoutubeRoomInfo::roomHandle()
{
    LiveInfo info;
    info.liverName = this->channelName;
    CDocument doc;
    doc.parse(response);
    CSelection ans = doc.find("html body[dir] script[nonce]");
    if (!ans.nodeAt(0).valid())
        return info;
    std::string r = ans.nodeAt(0).text();
    unsigned long indexFront = 0;
    unsigned long indexLast = r.size() - 1;
    while (r.front() != '{')        //越过前面的var ytInitialPlayerResponse
        ++indexFront;
    while (r.back() != '}')         //去掉后面可能的多余字符
        --indexLast;
    std::string res = r.substr(indexFront, indexLast - indexLast);
    //这个数组里面是一个json文件，利用json解析即可
    Json::Reader reader;
    Json::Value root;
    if (reader.parse(res, root))
    {
        if (root["videoDetails"]["isLive"].asBool())
        {
            info.status = ONLINE;
            info.title = root["videoDetails"]["title"].asString();
#ifdef __DEBUG__
            std::cout << info.title << std::endl;
#endif
        }
        else
            info.status = OFFLINE;
    }
    return info;
}