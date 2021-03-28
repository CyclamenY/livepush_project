//
// Created by angry on 2021/3/25.
//

#include "server.h"
#include <unistd.h>

void Server::httpRequest()
{
    CURL *nowcurl = nullptr;
    CURLM *curlm = curl_multi_init();
    CURLMsg *msg = nullptr;
    int stillRunning = 0;
    int msgsLeft = 0;
    while (1)
    {
        for (const auto &it:roomList)
            curl_multi_add_handle(curlm, it->curl);
        curl_multi_perform(curlm, &stillRunning);
        do
        {
            int numfds = 0;
            int res = curl_multi_wait(curlm, nullptr, 0, 10000, &numfds);
            if (res != CURLM_OK)
            {
                std::cout << "error: curl_multi_wait() returned" << res;
                return;
            }
            curl_multi_perform(curlm, &stillRunning);
        } while (stillRunning);
        while (msg = curl_multi_info_read(curlm, &msgsLeft))
        {
            if (msg->msg == CURLMSG_DONE && msg->data.result == CURLE_OK)
            {
                nowcurl = msg->easy_handle;
                LiveInfo info = roomMap[nowcurl]->roomHandle();
                if (info.status != roomMap[nowcurl]->liveStatus)
                {
                    if (info.status == ONLINE) //说明现在是上播
                        info.statusFlag = true;
                    else if (info.status == OFFLINE)   //说明现在是下播
                        info.statusFlag = false;
                    messageList.push_back(info);
                    roomMap[nowcurl]->liveStatus = info.status;
                    //TODO:唤醒线程
                }
                curl_multi_remove_handle(curlm, nowcurl);
            }
        }

//        for (const auto &it:roomList)
//        {
//            if (it->pushFlag && it->roomHandle())
//            {
//                //推一个值进messageList
//                messageList.push_back({it, sendKeyList.size()});
//                //TODO:唤醒所有线程
//            }
//        }
        sleep(60);
    }
}

void Server::livePush()
{

}

void Server::serverRun()
{
    httpRequest();
}

void *Server::livePushEnter(void *arg)
{
    auto *server = reinterpret_cast<Server *>(arg);
    server->livePush();
    return server;
}


