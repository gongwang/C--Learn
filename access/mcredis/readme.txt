对redis数据库进行的接口封装,依赖hiredis库，因为使用了多线程，所以依赖pthread库
另：uuid依赖libuuid库，安装方式：yum install libuuid-devel