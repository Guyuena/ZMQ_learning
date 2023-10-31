

#include <bits/stdc++.h>
#include "ZmqClientUtil.h"
#include "ZmqServerUtil.h"
using namespace std;
class TestClass{
private:
    whu::ZmqClientUtil *client;             //客户端指针
    whu::ZmqServerUtil *server;             //服务端指针
    std::thread* client_thread_;            //客户端线程
    std::thread* server_thread_;            //服务端线程
public:
    TestClass(){
        client=new whu::ZmqClientUtil(nullptr, nullptr,"tcp://127.0.0.1:8888");
        server=new whu::ZmqServerUtil(nullptr, nullptr,"tcp://127.0.0.1:8888");
    }
    ~TestClass(){
        delete client;
        delete server;
        delete client_thread_;
        delete server_thread_;
    }
    void CreatThread(){                     //创建线程
        client_thread_=new std::thread(&TestClass::Request,this);
        server_thread_=new std::thread(&TestClass::Service,this);
    }
    void JoinAll(){
        client_thread_->join();
        server_thread_->join();
    }
    void Request(){                         //客户端请求函数
        int i=1;
        string ServiceName="Say Hello";
        while(true){
            cout<<"Client Request "<<i<<" for "<<ServiceName<<endl;
            client->Request(ServiceName);
            cout<<"Client Received "<<i++<<" Msg : "<<client->receive_content_string<<endl;
            cout<<endl;
            Sleep(2000);                    //每隔2秒发送一次请求
        }
    }
    static std::string CreatString(){       //生成服务内容的函数
        static int i=1;
        return to_string(i++)+" Hello World!";
    }
    void Service(){
        int i=1;
        string ServiceName="Say Hello";
        while(true){
            server->Service(ServiceName,CreatString);
        }
    }
};
int main(){
    TestClass test;
    test.CreatThread();
    test.JoinAll();
    return 0;
}
