/**
 * @file publisher.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-31
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include<cpp/zmq.hpp>                               //c++的头文件专门用了一个单独的文件夹存放
#include<iostream>
#include<string>
int main(void){
    using namespace std;
    const string address="tcp://127.0.0.1:8888";    //发布地址
    void* context=zmq_ctx_new();                    //创建上下文，名称可以自定义，就是个变量
    void* socket=zmq_socket(context,ZMQ_PUB);       //创建套接字，名称可以自定义，就是个变量，类型为ZMQ_PUB表示为发布者
    int rc=zmq_bind(socket,address.c_str()); assert(rc==0); //绑定套接字到tcp协议的某个端口上，并对返回值进行断言判定
    int i=1;                                        //计数器
    void* buffer=malloc(256);                       //创建buffer，用于存放消息，假定最长不超过256个字符
    while(true){
        cout<<"Please enter your message:"<<endl;   //提示输入
        string msg_string;
        getline(cin,msg_string);                    //用于接收整行的消息
        if(msg_string=="end") break;                //如果接收到end字符串则停止接收消息，否则一直接收
        int size=msg_string.size();                 //将接收到的字符串复制到buffer中
        char *temp=(char*)buffer;
        for(int i=0;i<size;i++)
            temp[i]=msg_string[i];
        rc=zmq_send(socket,buffer,size,0); assert(rc==size);    //返回值为发送的数据字节数，使用send将buffer中的数据发送出去，需要指定发送的数据长度
        cout<<"Send NO."<<i++<<" message: "<<msg_string<<endl;  //提示消息已经发出
        Sleep(1);                                   //稍作停顿
    }
    zmq_close(socket);                              //结束后关闭套接字和上下文环境
    zmq_ctx_shutdown(context);
    return 0;
}
