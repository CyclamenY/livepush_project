//
// Created by angry on 2021/3/24.
//
#pragma once
#ifndef LIVEPUSH_JSONFILEREAD_H
#define LIVEPUSH_JSONFILEREAD_H

#include <iostream>
#include <list>
#include <curl/curl.h>
#include "../data/data.h"
#include "../server/server.h"
#include "../include/include.h"

class JsonFileRead
{
    bool cJsonTostruct(const std::string &roomFileName);

    bool cJsonToList(const std::string &sendKeyFileName);

public:
    JsonFileRead(Server &server, const std::string &roomFileName, const std::string &sendKeyfileName);

public:
    Server *server;
};


#endif //LIVEPUSH_JSONFILEREAD_H
