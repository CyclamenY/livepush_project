//
// Created by angry on 2021/3/24.
//

#include "jsonfileread.h"
#include <fstream>
#include "../cJson/cJSON.h"
#include <jsoncpp/json/json.h>
#include <cstring>

JsonFileRead::JsonFileRead(const std::string &fileName)
{
    cJsonTostruct(fileName);
}
bool JsonFileRead::cJsonTostruct(const std::string &fileName)
{
    Json::Reader reader;
    Json::Value root;

    //从文件中读取
    std::ifstream roomInfoFile(fileName, std::ios::binary);
    if( !roomInfoFile.is_open() )
    {
        std::cout << "Error opening file\n";
        return false;
    }
    if(reader.parse(roomInfoFile,root))
    {
        for(const auto& it:root["data"])
        {
            auto* room=new RoomInfo;
            room->channelName = it["channelName"].asString();
            room->roomId = std::to_string(it["roomId"].asInt());
            room->source = it["source"].asString();
            room->pushFlag = it["pushFlag"].asBool();
            roomList.push_back(room);
        }
    }
    return true;
}