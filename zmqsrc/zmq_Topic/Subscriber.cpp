/**
 * @file Subscriber.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#include <zmq.h>
#include <string>
#include <iostream>

int main() {
    void* context = zmq_ctx_new();
    void* subscriber = zmq_socket(context, ZMQ_SUB);
    zmq_connect(subscriber, "tcp://localhost:5555");

    // 订阅主题为"News"的消息
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "News", 4);
/*
ZeroMQ（ZMQ）中的不同通信模式在使用时可能需要不同的套接字选项。在ZMQ的Server-Client模式中，
通常不需要显式设置ZMQ_SUBSCRIBE或ZMQ_UNSUBSCRIBE套接字选项，因为Server和Client之间通常不涉及主题（Topic）的过滤。
它们主要用于双向通信，而不需要消息主题的筛选。

但在Publisher-Subscriber模式中，消息主题的过滤非常重要。发布者（Publisher）发布带有主题的消息，
而订阅者（Subscriber）可以选择性地订阅感兴趣的主题。这是因为在发布-订阅模式下，一个发布者可以同时发布多个不同主题的消息，
而订阅者可能只对其中一些主题感兴趣。因此，为了确保订阅者只接收其感兴趣的消息，需要使用ZMQ_SUBSCRIBE或ZMQ_UNSUBSCRIBE套接字选项来设置主题的过滤条件。
所以，发布者-订阅者模式中需要使用zmq_setsockopt来设置主题过滤条件，而Server-Client模式通常不需要进行这种类型的主题过滤，
因此在使用时不需要显式设置相关套接字选项。
*/




    while (true) {
        zmq_msg_t topic_msg;
        zmq_msg_t message_msg;

        // 创建一个空的 zmq_msg_t 结构
        zmq_msg_init(&topic_msg);
        zmq_msg_init(&message_msg);

        zmq_msg_recv(&topic_msg, subscriber, 0);
        zmq_msg_recv(&message_msg, subscriber, 0);

        std::string topic_str(static_cast<char*>(zmq_msg_data(&topic_msg)), zmq_msg_size(&topic_msg));
        std::string message_str(static_cast<char*>(zmq_msg_data(&message_msg)), zmq_msg_size(&message_msg));

        std::cout << "Received topic: " << topic_str << ", message: " << message_str << std::endl;

        zmq_msg_close(&topic_msg);
        zmq_msg_close(&message_msg);
    }

    return 0;
}

// g++ -o subscriber Subscriber.cpp -lzmq

// 报错
// /usr/bin/ld: /tmp/cczQ1Nc9.o: in function `main':
// Publisher.cpp:(.text+0x20): undefined reference to `zmq_ctx_new'
// /usr/bin/ld: Publisher.cpp:(.text+0x3b): undefined reference to `zmq_socket'        
// /usr/bin/ld: Publisher.cpp:(.text+0x5b): undefined reference to `zmq_bind'
// /usr/bin/ld: Publisher.cpp:(.text+0x118): undefined reference to `zmq_msg_init_data'
// /usr/bin/ld: Publisher.cpp:(.text+0x156): undefined reference to `zmq_msg_init_data'
// /usr/bin/ld: Publisher.cpp:(.text+0x174): undefined reference to `zmq_msg_send'     
// /usr/bin/ld: Publisher.cpp:(.text+0x18f): undefined reference to `zmq_msg_send'     
// collect2: error: ld returned 1 exit status

// 这个错误是由于编译器找不到ZeroMQ库文件引起的   编译的时候加上  -lzmq