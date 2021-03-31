//
// Created by angry on 2021/3/25.
//

#include "server.h"
#include <unistd.h>
#include <regex>

//size_t Server::req_reply(void *ptr, size_t size, size_t nmemb, void *stream)
//{
//    if (stream == nullptr || ptr == nullptr || size == 0)
//        return 0;
//    size_t realsize = size * nmemb;
//    auto *buffer = (std::string *)stream;
//    if (buffer != nullptr)
//        buffer->append((const char *)ptr, realsize);
//    return realsize;
//}

void Server::httpRequest()
{
    CURLM *curlm = curl_multi_init();
    CURL *nowcurl = nullptr;
    CURLMsg *msg = nullptr;
    int stillRunning = 0;
    int msgsLeft = 0;
    while (runFlag)
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
                    lock.lock();
                    messageList.push_back(info);
                    lock.unlock();
                    roomMap[nowcurl]->liveStatus = info.status;
                    //TODO:唤醒线程
                }
                curl_multi_remove_handle(curlm, nowcurl);
            }
        }
        sleep(60);
        //sleep(10);  //调试使用
    }
    curl_multi_cleanup(curlm);
}

void Server::livePush()
{
    LiveInfo info;
    CURLM *curlm = curl_multi_init();
    CURL *nowcurl = nullptr;
    CURLMsg *msg = nullptr;
    int stillRunning = 0;
    int msgsLeft = 0;
    std::string pushUrl;
    std::string response;
    while (runFlag)
    {
        lock.lock();
        if (messageList.empty())
        {
            lock.unlock();
            continue;
        }
        else
        {
            info = messageList.front();
            messageList.pop_front();
        }
        lock.unlock();
        for (const auto &it:pushCurlMap)
        {
            char *exLiver;
            char *exTitle;
            std::string temp;
            if (info.status == ONLINE)
            {
                temp = info.liverName + "开播啦！";
                exLiver = curl_easy_escape(nullptr, temp.c_str(), temp.size());
                exTitle = curl_easy_escape(nullptr, info.title.c_str(), info.title.size());
                pushUrl = "https://sctapi.ftqq.com/" + it.first + ".send?" + "title=" + exLiver + "&desp=" + exTitle;
                free(exTitle);
            }
            else
            {
                temp = info.liverName + "已下播";
                exLiver = curl_easy_escape(nullptr, temp.c_str(), temp.size());
                pushUrl = "https://sctapi.ftqq.com/" + it.first + ".send?" + "title=" + exLiver;
            }
            free(exLiver);
            CURL *curl = it.second;
            curl_easy_setopt(curl, CURLOPT_URL, pushUrl.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, reinterpret_cast<void *>(&response)); //设置写入字符串
            curl_multi_add_handle(curlm, curl);
            //free(exPushUrl);
        }
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
            std::cout << msg->data.result << std::endl;
            if (msg->msg == CURLMSG_DONE && msg->data.result == CURLE_OK)
            {
                std::cout << response << std::endl;
                nowcurl = msg->easy_handle;
                curl_multi_remove_handle(curlm, nowcurl);
                //free(exPushUrl);
            }
        }
        response.clear();
    }
    curl_multi_cleanup(curlm);
}

void Server::serverRun()
{
    pushThread = new std::thread(&Server::livePush, this);
    pushThread->detach();
    httpRequest();
}

void *Server::livePushEnter(void *arg)
{
    auto *server = reinterpret_cast<Server *>(arg);
    server->livePush();
    return server;
}


