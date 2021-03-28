//
// Created by angry on 2021/3/25.
//

#include "data.h"
#include <jsoncpp/json/json.h>

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

CURLcode RoomInfo::curlGetRequest(std::string &response)
{
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, reinterpret_cast<void *>(&response)); //设置写入字符串
    return curl_easy_perform(curl);
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
        if (root["data"]["live_status"].asInt() == 1)
        {
            info.status = ONLINE;
            info.title = root["data"]["title"].asString();
        } else
            info.status = OFFLINE;
    }
    return info;
}

LiveInfo YoutubeRoomInfo::roomHandle()
{
    CURLcode res;
    std::string result;
    if (curl)
    {
        res = curlGetRequest(liveStr);
        /* Check for errors */
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
            //return ERR;
        }
        //TODO:业务逻辑
    }
    //return ONLINE;
}