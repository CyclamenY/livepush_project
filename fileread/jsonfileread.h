//
// Created by angry on 2021/3/24.
//
#pragma once
#ifndef LIVEPUSH_JSONFILEREAD_H
#define LIVEPUSH_JSONFILEREAD_H

#include <iostream>
#include <list>
#include "../data/data.h"

class JsonFileRead
{
    bool cJsonTostruct(const std::string &fileName);
public:

    JsonFileRead(const std::string &fileName);
public:
    std::string roomInfo;
    std::list<RoomInfo *> roomList;
};


#endif //LIVEPUSH_JSONFILEREAD_H
