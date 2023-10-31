

//
// Created by songjiahao on 2021/11/29.
//
 
#ifndef ZMQSERVERUTIL_H
#define ZMQSERVERUTIL_H
 
#include <string>
// #include <cpp/zmq.hpp>
#include "include/zmq.h"
class ZmqServerUtil {
private:
    void *context;
    void *responder;
    std::string url;
 
public:
    ZmqServerUtil();  //默认构造方法
    ZmqServerUtil(void *context, void *responder, const std::string &url);   //初始化构造方法
    virtual ~ZmqServerUtil();     //析构函数
 
public:
    void Service(const std::string &ServiceName, std::string (*ServieceContentFunction)());
};
 
#endif
