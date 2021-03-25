//
// Created by angry on 2021/3/25.
//

#include "server.h"
#include <curl/curl.h>

void Server::httpRequest()
{
    while (1)
    {
        for (const auto &it:roomList)
        {
            if (it->pushFlag && it->roomHandle())
            {
                //推一个值进messageList
                messageList.push_back({it, sendKeyList.size()});
                //TODO:唤醒所有线程
            }
        }
    }
}

void Server::livePush()
{

}

void Server::serverRun()
{
    //这里开50个线程
    for (int i = 0; i < 50; ++i)
    {
        std::thread t(&Server::livePush, this);
        threadPool.emplace_back(&t);
        t.detach();
    }
}

void *Server::livePushEnter(void *arg)
{
    auto *server = reinterpret_cast<Server *>(arg);
    server->livePush();
    return server;
}

Server::Server(int threadPoolSize)
{
    threadPool = std::vector<std::thread *>(threadPoolSize);
}


