

 
#include "ZmqClientUtil.h"
ZmqClientUtil::ZmqClientUtil() {
 
}
ZmqClientUtil::ZmqClientUtil(void *context, void *requester, const std::string &url)
                            :context(context),requester(requester),url(url) {
    //初始化上下文context
    if (this->context == nullptr){
        this->context = zmq_ctx_new();
        assert(this->context != nullptr);
    }
 
    //获取socket对象
    if (this->requester == nullptr){                            //模式为请求响应
        this->requester = zmq_socket(this->context, ZMQ_REQ);   //如果响应者是空的句柄那就创建一个不透明的套接字...
        assert(this->requester != nullptr);
    }
 
    //socket绑 定通信地址  绑定特定的ip和端口号获取数据....
    //zmq_bind (responder, "tcp://*:5555");
    int result = zmq_connect(this->requester, this->url.c_str());
    assert(result==0);
}
 
ZmqClientUtil::~ZmqClientUtil() {
    zmq_close (requester);
    zmq_ctx_destroy (context);
}
 
void ZmqClientUtil::Request(const std::string & ServiceName) {
    int ret=zmq_send(this->requester,ServiceName.c_str(),ServiceName.size(),0);
    assert(ret==ServiceName.size());
    client_recv_data();
}
 
void ZmqClientUtil::client_recv_data() {
    const int MAXLEN=1024;
    char buffer[MAXLEN]= {};
    int ret=zmq_recv(this->requester,buffer,MAXLEN,0);
    assert(ret!=-1);
    std::string result;
    for(int i=0;i<ret;i++)
        result.push_back(buffer[i]);
    receive_content_string=result;
}
