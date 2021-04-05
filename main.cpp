#include <curl/curl.h>
#include "./data/data.h"
#include "./server/server.h"
#include "./fileread/jsonfileread.h"
#include <csignal>
#include "./include/include.h"

using namespace std;

bool Server::runFlag = true;

int main()
{
    curl_global_init(CURL_GLOBAL_ALL);
    signal(SIGINT | SIGTERM, [](int)
    { Server::runFlag = false; });
    Server server;
    JsonFileRead jsonRead(server, "../roominfo.json", "../sendkey.json");
    server.serverRun();
    //清理操作
    curl_global_cleanup();      //清理内存
    return 0;
}