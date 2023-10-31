

#include "ZmqRecvUtil.h"
 
ZmqRecvUtil::ZmqRecvUtil() {}
 
ZmqRecvUtil::ZmqRecvUtil(void *context, void *subscriber, const std::string &url) 
                        :context(context),subscriber(subscriber),url(url){
    //初始化上下文context
    if (this->context == nullptr){
        this->context = zmq_ctx_new();  //如果上下文是空的，那么就初始化上下文
        assert(this->context != nullptr);
    }
 
    //获取socket对象
    if (this->subscriber == nullptr){   //模式为订阅发布模式 发布端单向发布数据也不管订阅端是否能接收...
        this->subscriber = zmq_socket(this->context, ZMQ_SUB); //如果订阅者是空的句柄那就创建一个不透明的套接字...
        assert(this->subscriber != nullptr);
    }
 
    //socket绑定通信地址  绑定特定的ip和端口号获取数据....
    int result = zmq_connect(this->subscriber, this->url.c_str());
    assert(result == 0);
    /*
     int zmq_setsockopt (void *socket, int option_name, const void *option_value, size_t option_len);
     注意：除了以下属性，所有的属性均需要在对socket进行bind/connect操作之前设置：
     ZMQ_SUBSCRIBE, ZMQ_UNSUBSCRIBE, ZMQ_LINGER, ZMQ_ROUTER_HANDOVER,
     ZMQ_ROUTER_MANDATORY, ZMQ_PROBE_ROUTER, ZMQ_XPUB_VERBOSE, ZMQ_REQ_CORRELATE, and ZMQ_REQ_RELAXED
     特别的，安全的属性也可以在bind/connect操作之后生效，并且可以随时进行修改并影响之后的bind/connect操作。*/
    //设置socket可选项,其中“”参数为过滤器，用于过滤获取信息，这里默认全部接收
    result = zmq_setsockopt(this->subscriber, ZMQ_SUBSCRIBE, "", 0);
    assert(result == 0);
}
 
ZmqRecvUtil::~ZmqRecvUtil() {
    zmq_close(this->subscriber);
    zmq_ctx_destroy(this->context);
}
 
std::string ZmqRecvUtil::recvMsg() {
    //读取消息
    char infoBuffer[MAX_INFO_SIZE] = {0};
    int ret = zmq_recv(this->subscriber, infoBuffer, MAX_INFO_SIZE , 0);
    assert(ret != -1);
    std::string tmp="";                     //返回值不能使用infoBuffer强化类型转换，要重新构建字符串
    for (int i = 0; i < ret; ++i) {
        tmp+=infoBuffer[i];
    }
    return tmp;
}
