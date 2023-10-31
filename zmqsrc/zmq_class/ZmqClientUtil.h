


#ifndef ZMQCLIENTUTIL_H
#define ZMQCLIENTUTIL_H
 
#include <string>
#include <iostream>
#include <cassert>
// #include <cpp/zmq.hpp>
#include "include/zmq.h"
class ZmqClientUtil {
private:
    void *context;
    void *requester;
    std::string url;
 
public:
    std::string receive_content_string;
 
public:
    ZmqClientUtil();                //默认构造方法
    ZmqClientUtil(void *context, void *requester, const std::string &url);   //初始化构造方法
    virtual ~ZmqClientUtil();       //析构函数
 
public:
    void Request(const std::string &ServiceName);
 
    void client_recv_data();
};
 
#endif
