//
// Created by angry on 2021/3/24.
//
#pragma once
#ifndef LIVEPUSH_JSONFILEREAD_H
#define LIVEPUSH_JSONFILEREAD_H

#include <iostream>
#include <list>
#include "../data/data.h"
#include "../server/server.h"

class JsonFileRead
{
    bool cJsonTostruct(const std::string &roomFileName);

    bool cJsonToList(const std::string &sendKeyFileName);

public:
    JsonFileRead(Server &server, const std::string &roomFileName, const std::string &sendKeyfileName);

public:
    Server *server;
    std::string roomInfo;
    std::list<RoomInfo *> roomList;
    std::list<std::string> sendKeyList;
};


#endif //LIVEPUSH_JSONFILEREAD_H
