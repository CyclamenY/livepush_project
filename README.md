# livepush_project
 一款bilibili,youtube,斗鱼开播提醒工具

需要的东西
--------

* jsoncpp，因为需要解析房间名和id的json文件
  * centos系统运行时可能要将源文件中所有```#include <jsoncpp\json\json.h>```修改为```#include <json\json.h>```，当然也可以在```/usr/local/include/json```外面套一层```jsoncpp```文件夹
* cmake与make，本项目使用cmake编译
* libcurl，C\+\+HTTPS请求库，用于轮询B站和油管接口以及Server酱接口
* Server酱账号
* gumbo-parser 谷歌的一个纯C HTML解析库
* gumbo-query 第三方的gumbo C++封装库

如何编译？
--------

### 建议在编译前把```include\include.h```中的```#define __BEBUG__```注释掉，可以加快运行速度


* 第一种方法
    - 在根目录下直接使用
        ```C++
        cmake && make
        ```

* 第二种方法
    - 创建一个新的目录，例如：`build`，并且将根目录中的`root`文件夹复制进来，在其中
        ```C++
        cmake ../ && make
        ```

更新日志
-------

- `2021.3.24` 项目开始，完成json解析
- `2021.3.25` 框架基本完成
- `2021.3.28` 换用multi_curl接口，轮询频道侧完成
- `2021.3.29` 推送侧完成，项目已可用
- `2021.3.31` 修复多次轮询返回相同结果的问题，增加对Ctrl+C信号的处理（试运行）
- `2021.4.5` 更新，完成youtube查询逻辑（还没有测试），项目基本完工
- `2021.5.18` 更新，增加对斗鱼直播间的支持（未测试）

需要完善的事情
-------

* ~~首要任务当然是完成整个代码了~~
* ~~完成条件变量编写~~
