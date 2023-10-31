

#ifndef ZMQSENDUTIL_H
#define ZMQSENDUTIL_H
 
#include <string>
// #include <cpp/zmq.hpp>
#include "include/zmq.h"
class ZmqSendUtil{
private:
    void *context;
    void *publisher;
    std::string url;
 
public:
    ZmqSendUtil();                              //默认构造方法
    ZmqSendUtil(void *context, void *publisher, const std::string &url);    //初始化构造方法
    virtual ~ZmqSendUtil();                     //析构函数
    int sendMsg(const std::string& info);       //发送消息
};
 
#endif
