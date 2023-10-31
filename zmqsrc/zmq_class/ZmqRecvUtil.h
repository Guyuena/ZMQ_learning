
#ifndef USV_ZMQRECVUTIL_H
#define USV_ZMQRECVUTIL_H
 
#include <string>
// #include <cpp/zmq.hpp>
#include "include/zmq.h"
class ZmqRecvUtil{
private:
    void *context;
    void *subscriber;
    std::string url;
 
public:
    ZmqRecvUtil();              //默认构造方法
    ZmqRecvUtil(void *context, void *subscriber, const std::string &url);   //初始化构造方法
    virtual ~ZmqRecvUtil();     //析构函数
    std::string recvMsg();      //接收消息
};
 
#endif
