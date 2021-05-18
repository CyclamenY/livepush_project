//
// Created by angry on 2021/3/25.
//

#include "data.h"
#include <jsoncpp/json/json.h>
#include <iostream>
#include <gq/Document.h>
#include <gq/Node.h>
#include <iconv.h>
#include <cstring>

std::string UnicodeToUtf8(char *sourceData)
{
    iconv_t cd = iconv_open("UTF-8", "UNICODE//IGNORE");
    if (cd == (iconv_t)-1)
        perror("iconv_open");
    size_t srclen = strlen(sourceData);

    /* 存放转换后的字符串 */
    size_t outlen = 1024;
    char outbuf[outlen];
    memset(outbuf, 0, outlen);

    /* 由于iconv()函数会修改指针，所以要保存源指针 */
    char *srcstart = sourceData;
    char *tempoutbuf = outbuf;

    /* 进行转换
     *@param cd iconv_open()产生的句柄
     *@param srcstart 需要转换的字符串
     *@param srclen 存放还有多少字符没有转换
     *@param tempoutbuf 存放转换后的字符串
     *@param outlen 存放转换后,tempoutbuf剩余的空间
     *
     * */
    size_t ret = iconv(cd, &srcstart, &srclen, &tempoutbuf, &outlen);
    if (ret == -1)
        perror("iconv");
    return outbuf;
}

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
#ifdef __DEBUG__
        std::cout << this->channelName << " live_status:" << root["data"]["live_status"].asInt() << std::endl;
#endif
        if (root["data"]["live_status"].asInt() == 1)
        {
            info.status = ONLINE;
            info.title = root["data"]["title"].asString();
#ifdef __DEBUG__
            std::cout << info.title << std::endl;
#endif
        }
        else
            info.status = OFFLINE;
    }
    response.clear();
    return info;
}

LiveInfo YoutubeRoomInfo::roomHandle()
{
    LiveInfo info;
    info.liverName = this->channelName;
    CDocument doc;
    doc.parse(response);
    CSelection ans = doc.find("html body[dir] script[nonce]");
    if (!ans.nodeAt(0).valid())
        return info;
    std::string r = ans.nodeAt(0).text();
    unsigned long indexFront = 0;
    unsigned long indexLast = r.size() - 1;
    while (r.front() != '{')        //越过前面的var ytInitialPlayerResponse
        ++indexFront;
    while (r.back() != '}')         //去掉后面可能的多余字符
        --indexLast;
    std::string res = r.substr(indexFront, indexLast - indexLast);
    //这个数组里面是一个json文件，利用json解析即可
    Json::Reader reader;
    Json::Value root;
    if (reader.parse(res, root))
    {
        if (root["videoDetails"]["isLive"].asBool())
        {
            info.status = ONLINE;
            info.title = root["videoDetails"]["title"].asString();
#ifdef __DEBUG__
            std::cout << info.title << std::endl;
#endif
        }
        else
            info.status = OFFLINE;
    }
    return info;
}

LiveInfo DouyuRoomInfo::roomHandle()
{
    Json::Reader reader;
    Json::Value root;
    LiveInfo info;
    info.liverName = this->channelName;
    if (reader.parse(response, root))
    {
        if (root["Rendata"]["tname"].asString() == "not_find")
            info.status = ERR;
#ifdef __DEBUG__
        std::cout << this->channelName << " live_status:" << root["Rendata"]["media_type"].asString() << std::endl;
#endif
        if (root["Rendata"]["media_type"].asString() == "live")
        {
            info.status = ONLINE;
            char *inbuf = const_cast<char *>(root["Rendata"]["roomName"].asCString());
            info.title = UnicodeToUtf8(inbuf);
#ifdef __DEBUG__
            std::cout << info.title << std::endl;
#endif
        }
        else
            info.status = OFFLINE;
    }
    response.clear();
    return info;
}