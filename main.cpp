#include <curl/curl.h>
#include <iostream>
#include "./data/data.h"
#include "./server/server.h"
#include "./fileread/jsonfileread.h"

using namespace std;


int main()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    Server server;
    JsonFileRead jsonRead(server, "../roominfo.json", "../sendkey.json");
    server.serverRun();
//    const string title = "推送示例";
//    const string desp = "这是推送内容";
//    const string id = "SCT22617TjiTJYpbYXawtRxT9CpjfKgSO";
//    const string httpsstr = "https://sctapi.ftqq.com/" + id + ".send?title=" + title + "&desp=" + desp;
//    const string room_id = "1133";
//    string test = "https://api.live.bilibili.com/room/v1/Room/get_info?room_id=" + room_id;
//    CURL *curl;
//    CURLcode res;
//
//    curl_global_init(CURL_GLOBAL_DEFAULT);
//
//    curl = curl_easy_init();
//    if (curl)
//    {
//        curl_easy_setopt(curl, CURLOPT_URL, test.c_str());
//
//        /* Perform the request, res will get the return code */
//        res = curl_easy_perform(curl);
//        /* Check for errors */
//        if (res != CURLE_OK)
//            fprintf(stderr, "curl_easy_perform() failed: %s\n",
//                    curl_easy_strerror(res));
//
//        /* always cleanup */
//        curl_easy_cleanup(curl);
//    }
//
//    curl_global_cleanup();

    //清理操作
    curl_global_cleanup();      //清理内存
    return 0;
}