//
// Created by angry on 2021/3/24.
//
#pragma once

#ifndef LIVEPUSH_DATA_H
#define LIVEPUSH_DATA_H

#include <string>
#include <curl/curl.h>

//struct sendkey    //单个key，不需要struct，先放着
class RoomInfo
{
public:
    RoomInfo(const std::string &channelName, const std::string &roomId, const std::string &source, bool pushFlag,
             CURL *curl) :
            channelName(channelName), roomId(roomId), source(source), pushFlag(pushFlag), curl(curl)
    {
        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
    };

    virtual ~RoomInfo()
    {
        /* always cleanup */
        curl_easy_cleanup(curl);    //关闭连接
        curl_global_cleanup();      //清理内存
    }

    virtual bool roomHandle()
    {};
public:
    std::string channelName;
    std::string roomId;
    std::string source;
    bool pushFlag;
    CURL *curl;
private:
private:
};

class BilibiliRoomInfo : public RoomInfo    //bilibili房间信息
{
public:
    std::string liveStr = "https://api.live.bilibili.com/room/v1/Room/get_info?room_id=";
public:
    bool roomHandle();
};

class YoutubeRoomInfo : public RoomInfo     //youtube房间信息，由于网络环境，这个可能用不到
{
public:
    std::string liveStr = "https://www.youtube.com/channel/";
public:
    bool roomHandle();
};

bool BilibiliRoomInfo::roomHandle()
{
    CURLcode res;
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, liveStr.c_str());
        //TODO:业务逻辑

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
    }
}

bool YoutubeRoomInfo::roomHandle()
{
    CURLcode res;
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, liveStr.c_str());
        //TODO:业务逻辑

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
    }
}

#endif //LIVEPUSH_DATA_H
