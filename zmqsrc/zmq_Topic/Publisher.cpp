/**
 * @file Publisher.cpp
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
#include <chrono> // 添加头文件
#include <thread>

int main() {
    void* context = zmq_ctx_new();
    void* publisher = zmq_socket(context, ZMQ_PUB);
    zmq_bind(publisher, "tcp://*:5555");

    // 发布主题为"News"的消息
    while (true) {
        std::string topic = "News";
        std::string message = "This is a news message.";

        zmq_msg_t topic_msg;
        // 初始化数据
        zmq_msg_init_data(&topic_msg, (void*)topic.c_str(), topic.length(), NULL, NULL);

        zmq_msg_t message_msg;
        zmq_msg_init_data(&message_msg, (void*)message.c_str(), message.length(), NULL, NULL);
        // zmq_msg_send() 是一个更底层的函数，它接受一个 zmq_msg_t 类型的消息结构作为参数，可以包含消息的内容、
        // 长度和其他属性。
        zmq_msg_send(&topic_msg, publisher, ZMQ_SNDMORE);
        zmq_msg_send(&message_msg, publisher, 0);

        std::this_thread::sleep_for(std::chrono::seconds(1)); // 使用std::this_thread
    }

    return 0;
}

// g++ -o publisher Publisher.cpp -lzmq