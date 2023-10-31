#include <zmq.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    // ZMQ 上下文是 ZeroMQ 库中的全局环境，用于管理套接字、线程和其他资源。
    // 上下文在 ZeroMQ 库中是必须的，并且通常只需要一个全局上下文，因此它是一个 void* 类型指针。
    void* context = zmq_ctx_new();
    // Pusher 创建一个 PUSH 套接字，将任务推送到任务队列
    // 定义了套接字的类型和行为，用于指定如何处理消息的发送和接收
    // 据套接字类型定义了其后续的消息传递行为。
    void* pusher = zmq_socket(context, ZMQ_PUSH);
    // 用于将套接字绑定到一个地址，以便等待来自其他套接字的连接或消息
    zmq_bind(pusher, "tcp://*:5555");

    int taskNumber = 1;

    while (1) {
        char task[256];
        sprintf(task, "Task %d", taskNumber);

        zmq_msg_t msg;
        // 初始化数据结构的大小
        zmq_msg_init_size(&msg, strlen(task));
        memcpy(zmq_msg_data(&msg), task, strlen(task));

        zmq_msg_send(&msg, pusher, 0);
        zmq_msg_close(&msg);

        printf("Pushed: %s\n", task);

        taskNumber++;
        sleep(1);
    }

    zmq_close(pusher);
    zmq_ctx_destroy(context);

    return 0;
}
