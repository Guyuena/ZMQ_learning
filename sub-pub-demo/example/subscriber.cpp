/**
 * @file subscriber.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-31
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include<cpp/zmq.hpp>
#include<iostream>
int main(void){
    using namespace std;
    const string address="tcp://127.0.0.1:8888";                //确定接收地址
    void* context=zmq_ctx_new();                                //创建上下文
    void* socket=zmq_socket(context,ZMQ_SUB);                   //创建套接字，类型为ZMQ_SUB表示为订阅者
    int rc=zmq_connect(socket,address.c_str()); assert(rc==0);  //链接到发布者发布的地址上，并对返回值进行断言判定
    rc = zmq_setsockopt (socket, ZMQ_SUBSCRIBE, "", 0); assert(rc==0);//设置订阅该端口上的所有信息，没有这一句，则收不到消息，可以使用过滤器过滤接收
    int i=1;                                                    //计数器
    void* buffer=malloc(256);                                   //创建buffer，用于接收消息
    while(true){
        rc=zmq_recv(socket,buffer,256,0); assert(rc!=-1);       //接收一条消息
        int size=rc;
        char *rec_msg=(char*)malloc(sizeof(size+1));            //创建一个字符数组，用于存放消息
        memcpy(rec_msg,buffer,size); rec_msg[size]='\0';        //构建字符串
        cout<<"Receive NO."<<i++<<" message: "<<rec_msg<<endl;  //输出消息
        free(rec_msg);                                          //释放字符数字空间
    }
    zmq_close(socket);                                          //由于循环不会结束，所以以下两句并不会执行
    zmq_ctx_shutdown(context);                                  //但是依然要有回收清理的收尾习惯
    return 0;
}
