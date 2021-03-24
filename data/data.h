//
// Created by angry on 2021/3/24.
//
#pragma once

#ifndef LIVEPUSH_DATA_H
#define LIVEPUSH_DATA_H



//struct sendkey    //单个key，不需要struct，先放着
struct RoomInfo
{
    std::string channelName;
    std::string roomId;
    std::string source;
    bool pushFlag;
};
struct BilibiliRoomInfo: public RoomInfo {};    //bilibili房间信息
struct YoutubeRoomInfo: public RoomInfo {};     //youtube房间信息，由于网络环境，这个可能用不到


#endif //LIVEPUSH_DATA_H
