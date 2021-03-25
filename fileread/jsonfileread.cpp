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
            CURL *curl;
            auto *room = new RoomInfo(it["channelName"].asString(),std::to_string(it["roomId"].asInt()),it["source"].asString(),it["pushFlag"].asBool(),curl);
            if (room->source == "bilibili")
            {
                reinterpret_cast<BilibiliRoomInfo *>(room)->liveStr += room->roomId;
                server->roomList.push_back(reinterpret_cast<BilibiliRoomInfo *>(room));
            }
            else if (room->source == "youtube")
            {
                reinterpret_cast<YoutubeRoomInfo *>(room)->liveStr += (room->roomId + "live");
                server->roomList.push_back(reinterpret_cast<YoutubeRoomInfo *>(room));
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
            server->sendKeyList.emplace_back(it.asString());
    }
    sendKeyInfoFile.close();
    return true;
}
