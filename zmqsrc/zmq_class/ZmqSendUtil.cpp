

#include "ZmqSendUtil.h"
ZmqSendUtil::ZmqSendUtil() {}
ZmqSendUtil::ZmqSendUtil(void *context, void *publisher, const std::string &url) 
                        :context(context),publisher(publisher), url(url){
    //初始化上下文
    if (this->context == nullptr){
        this->context = zmq_ctx_new();
        assert(this->context != nullptr);
    }
 
    //获取socket对象
    if (this->publisher == nullptr){
        this->publisher = zmq_socket(this->context, ZMQ_PUB);
        assert(this->publisher != nullptr);
    }
 
    //socket绑定通信地址
    int result = zmq_bind(this->publisher, this->url.c_str());
    assert(result == 0);
}
 
ZmqSendUtil::~ZmqSendUtil() {
    zmq_close(this->publisher);
    zmq_ctx_destroy(this->context);
}
 
int ZmqSendUtil::sendMsg(const std::string& info) {
    int result=-1;
    if (this->context != nullptr && this->publisher != nullptr){
        result = zmq_send(this->publisher, info.c_str(), info.length(), 0);
    return result > 0;
}
