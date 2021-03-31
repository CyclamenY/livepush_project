//
// Created by angry on 2021/3/24.
//

#include "jsonfileread.h"
#include <fstream>
#include <jsoncpp/json/json.h>

JsonFileRead::JsonFileRead(Server &server, const std::string &roomFileName, const std::string &sendKeyFileName)
{
    this->server = &server;
    cJsonTostruct(roomFileName);
    cJsonToList(sendKeyFileName);
}

bool JsonFileRead::cJsonTostruct(const std::string &roomFileName)
{
    Json::Reader reader;
    Json::Value root;

    //从文件中读取房间配置文件
    std::ifstream roomInfoFile(roomFileName, std::ios::binary);
    if (!roomInfoFile.is_open())
    {
        std::cout << "Error opening file\n";
        return false;
    }
    if (reader.parse(roomInfoFile, root))
    {
        for (const auto &it:root["data"])
        {
            CURL *curl = curl_easy_init();
            if (it["source"] == "bilibili")
            {
                auto *room = new BilibiliRoomInfo(it["channelName"].asString(), std::to_string(it["roomId"].asInt()),
                                                  it["source"].asString(), it["pushFlag"].asBool(), curl);
                server->roomList.push_back(room);
                server->roomCurlMapPush(curl, room);
                server->roomCurlVecPush(curl);
            }
            else if (it["source"] == "youtube")
            {
                auto *room = new YoutubeRoomInfo(it["channelName"].asString(), std::to_string(it["roomId"].asInt()),
                                                 it["source"].asString(), it["pushFlag"].asBool(), curl);
                room->liveStr += (room->roomId + "/live");
                server->roomList.push_back(room);
                server->roomCurlMapPush(curl, room);
                server->roomCurlVecPush(curl);
            }
        }
    }
    roomInfoFile.close();
    return true;
}

bool JsonFileRead::cJsonToList(const std::string &sendKeyFileName)
{
    Json::Reader reader;
    Json::Value root;

    //从文件中读取推送密钥
    std::ifstream sendKeyInfoFile(sendKeyFileName, std::ios::binary);
    if (!sendKeyInfoFile.is_open())
    {
        std::cout << "Error opening file\n";
        return false;
    }
    if (reader.parse(sendKeyInfoFile, root))
    {
        for (const auto &it:root["sendkey"])
        {
            server->sendKeyList.emplace_back(it.asString());
            CURL *curl = curl_easy_init();
            curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, RoomInfo::req_reply);//回调函数
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
            curl_easy_setopt(curl, CURLOPT_DNS_CACHE_TIMEOUT, 0);
            server->pushCurlMapPush(it.asString(), curl);
        }
    }
    sendKeyInfoFile.close();
    return true;
}
