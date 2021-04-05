//
// Created by angry on 2021/3/24.
//
#pragma once

#ifndef LIVEPUSH_DATA_H
#define LIVEPUSH_DATA_H

#include <string>
#include <utility>
#include <curl/curl.h>
#include "../include/include.h"

enum LiveStatus
{
    ERR,
    ONLINE,
    OFFLINE
};

struct LiveInfo     //直播间消息
{
    LiveStatus status;
    std::string title;
    std::string liverName;
    bool statusFlag;
};

//struct sendkey    //单个key，不需要struct，先放着
class RoomInfo
{
public:
    RoomInfo(std::string channelName, std::string roomId, std::string source, bool pushFlag,
             CURL *curl) :
            channelName(std::move(channelName)), roomId(std::move(roomId)), source(std::move(source)),
            pushFlag(pushFlag), curl(curl)
    {
        liveStatus = OFFLINE;
        curl_easy_setopt(curl, CURLOPT_DNS_CACHE_TIMEOUT, 0);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);                       //回调函数
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, reinterpret_cast<void *>(&response)); //设置写入字符串
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    };

    virtual ~RoomInfo()
    {
        /* always cleanup */
        curl_easy_cleanup(curl);    //关闭连接
    }

    virtual LiveInfo roomHandle() = 0;

    static size_t req_reply(void *ptr, size_t size, size_t nmemb, void *stream);

public:
    std::string channelName;
    std::string roomId;
    std::string source;
    bool pushFlag;
    CURL *curl;
    std::string response;
    LiveStatus liveStatus;
private:
private:
};

class BilibiliRoomInfo : public RoomInfo    //bilibili房间信息
{
public:
    std::string liveStr = "https://api.live.bilibili.com/room/v1/Room/get_info?room_id=";
public:
    BilibiliRoomInfo(std::string channelName, std::string roomId, std::string source, bool pushFlag,
                     CURL *curl) : RoomInfo(std::move(channelName), std::move(roomId), std::move(source), pushFlag,
                                            curl)
    {
        liveStr += this->roomId;
        curl_easy_setopt(curl, CURLOPT_URL, liveStr.c_str()); //设置网址
    };

    LiveInfo roomHandle() override;
};

class YoutubeRoomInfo : public RoomInfo     //youtube房间信息，由于网络环境，这个可能用不到
{
public:
    std::string liveStr = "https://www.youtube.com/channel/";
public:
    YoutubeRoomInfo(std::string channelName, std::string roomId, std::string source, bool pushFlag,
                    CURL *curl) : RoomInfo(std::move(channelName), std::move(roomId), std::move(source), pushFlag, curl)
    {
        liveStr += (this->roomId + "/live");
        curl_easy_setopt(curl, CURLOPT_URL, liveStr.c_str()); //设置网址
    };

    LiveInfo roomHandle() override;
};

#endif //LIVEPUSH_DATA_H
