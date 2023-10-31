#include <zmq.h>
#include <stdio.h>
#include <string.h>

int main() {
    void* context = zmq_ctx_new();
    void* subscriber = zmq_socket(context, ZMQ_SUB);
    zmq_connect(subscriber, "tcp://localhost:5555");

    // 订阅您感兴趣的主题
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "Topic1", 6);
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "Topic2", 6);

    while (1) {
        zmq_msg_t topic;
        zmq_msg_t message;

        zmq_msg_init(&topic);
        zmq_msg_init(&message);

        zmq_msg_recv(&topic, subscriber, 0);
        zmq_msg_recv(&message, subscriber, 0);

        char* topic_str = (char*)zmq_msg_data(&topic);
        char* message_str = (char*)zmq_msg_data(&message);

        printf("Received topic: %s, message: %s\n", topic_str, message_str);

        zmq_msg_close(&topic);
        zmq_msg_close(&message);
    }

    zmq_close(subscriber);
    zmq_ctx_destroy(context);

    return 0;
}
