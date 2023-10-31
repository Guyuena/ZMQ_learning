#include <zmq.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    void* context = zmq_ctx_new();
    void* publisher = zmq_socket(context, ZMQ_PUB);
    zmq_bind(publisher, "tcp://*:5555");

    const char* topic = "Topic1"; // 更改为不同的主题
    const char* publisherName = "Publisher1"; // 更改为发布者名称

    while (1) {
        const char* message = "Message from Publisher1";

        zmq_msg_t topic_msg;
        zmq_msg_t message_msg;

        zmq_msg_init_size(&topic_msg, strlen(topic));
        memcpy(zmq_msg_data(&topic_msg), topic, strlen(topic));

        zmq_msg_init_size(&message_msg, strlen(message));
        memcpy(zmq_msg_data(&message_msg), message, strlen(message));

        zmq_msg_send(&topic_msg, publisher, ZMQ_SNDMORE);
        zmq_msg_send(&message_msg, publisher, 0);

        zmq_msg_close(&topic_msg);
        zmq_msg_close(&message_msg);

        sleep(1);
    }

    zmq_close(publisher);
    zmq_ctx_destroy(context);

    return 0;
}
