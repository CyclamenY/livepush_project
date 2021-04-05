//
// Created by angry on 2021/3/25.
//

#include "server.h"
#include <unistd.h>
#include <regex>

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
        msg = curl_multi_info_read(curlm, &msgsLeft);
        while (msg)
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
                    std::unique_lock<std::mutex> locker(lock);
                    messageList.push_back(info);
                    locker.unlock();
                    roomMap[nowcurl]->liveStatus = info.status;
#ifdef __DEBUG__
                    std::cout << info.liverName << (info.status ? "开播推入消息队列" : "下播推入消息队列");
#endif
                    cond.notify_one();
                }
                curl_multi_remove_handle(curlm, nowcurl);
            }
            msg = curl_multi_info_read(curlm, &msgsLeft);
        }
#ifdef __DEBUG__
        sleep(10);  //调试使用
#else
        sleep(60);
#endif
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
    while (runFlag || !messageList.empty())
    {
        std::unique_lock<std::mutex> locker(lock);
        while (messageList.empty())
            cond.wait(locker);
        info = messageList.front();
        messageList.pop_front();
        locker.unlock();
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
        msg = curl_multi_info_read(curlm, &msgsLeft);
        while (msg)
        {
            if (msg->msg == CURLMSG_DONE && msg->data.result == CURLE_OK)
            {
                nowcurl = msg->easy_handle;
#ifdef __DEBUG__
                std::cout << "发送一条消息" << std::endl;
#endif
                curl_multi_remove_handle(curlm, nowcurl);
                //free(exPushUrl);
            }
            msg = curl_multi_info_read(curlm, &msgsLeft);
        }
        response.clear();
    }
    curl_multi_cleanup(curlm);
}

void Server::serverRun()
{
    pushThread = new std::thread(&Server::livePush, this);
    httpRequest();
    pushThread->join();
}


