

//
// Created by songjiahao on 2021/11/29.
//
#include "ZmqServerUtil.h"
ZmqServerUtil::ZmqServerUtil() {}
 
ZmqServerUtil::ZmqServerUtil(void *context, void *responder, const std::string &url)
                            :context(context),responder(responder),url(url) {
    //初始化上下文context
    if (this->context == nullptr){
        this->context = zmq_ctx_new();
        assert(this->context != nullptr);
    }
 
    //获取socket对象
    if (this->responder == nullptr){                            //模式为请求响应
        this->responder = zmq_socket(this->context, ZMQ_REP);   //如果响应者是空的句柄那就创建一个不透明的套接字...
        assert(this->responder != nullptr);
    }
 
    //socket绑定通信地址  绑定特定的ip和端口号获取数据....
    //zmq_bind (responder, "tcp://*:5555");
    int result = zmq_bind (this->responder, this->url.c_str());
    assert(result == 0);
}
 
ZmqServerUtil::~ZmqServerUtil() {
    zmq_close (responder);
    zmq_ctx_destroy (context);
}
//参数为服务名称和生成服务内容的函数指针
void ZmqServerUtil::Service(const std::string & ServiceName, std::string (*ServieceContentFunction)()) {
    bool start=true;
    const int MAXLEN = 1024;
    while(start) {
        char buffer[MAXLEN] = {};                       //用于接收请求服务的名称
        int ret = zmq_recv(this->responder, buffer, MAXLEN, 0);
        assert(ret != -1);
        std::string quaryName;                          //将请求服务的名称转换为string，为避免截断，逐字符处理
        for (int i = 0; i < ret; i++)
            quaryName.push_back(buffer[i]);
        Sleep(10);
        if (quaryName == ServiceName) {                 //根据请求服务的名声回复内容
            std::string ServieceContent=ServieceContentFunction();
            ret = zmq_send(this->responder, ServieceContent.c_str(), ServieceContent.size(), 0);
            assert(ret = ServieceContent.size());
        }
    }
}
