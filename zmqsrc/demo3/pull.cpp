
/**
 * @file pull.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */
// 在ZeroMQ中，PULL 和 PUSH 模式用于实现工作队列或任务分发的场景。
// PUSH 套接字将任务推送给工作者（PULL 套接字），从而实现任务的分发
#include <zmq.h>
#include <stdio.h>
#include <string.h>

int main() {
    void* context = zmq_ctx_new();
    // Puller 创建一个 PULL 套接字，接收任务
    void* puller = zmq_socket(context, ZMQ_PULL);
    zmq_connect(puller, "tcp://localhost:5555");

    while (1) {
        zmq_msg_t msg;
        zmq_msg_init(&msg);

        zmq_msg_recv(&msg, puller, 0);

        char* task = (char*)zmq_msg_data(&msg);
        printf("Received: %s\n", task);

        zmq_msg_close(&msg);
    }

    zmq_close(puller);
    zmq_ctx_destroy(context);

    return 0;
}
