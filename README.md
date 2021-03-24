# livepush_project
 一款bilibili,youtube开播提醒工具

需要的东西
--------

* jsoncpp，因为需要解析房间名和id的json文件
* cmake与make，本项目改用cmake编译
* libcurl，C\+\+HTTPS请求库，用于轮询B站和油管接口以及Server酱接口
* Server酱账号
* 暂时这么多，想到再写

如何编译？
--------

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

需要完善的事情
-------

* 首要任务当然是完成整个代码了

## 代码逐步完善中......