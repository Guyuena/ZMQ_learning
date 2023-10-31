// 连接REQ套接字⾄ tcp://localhost:5555
 // 发送Hello给服务端，并接收World
//
// Hello World client
#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
//编译：gcc -o hwclient hwclient.c -lzmq
//编译：gcc -o client client.c -lzmq
int main (void)
{
	 printf ("Connecting to hello world server...\n");
	 void *context = zmq_ctx_new ();
	// 连接⾄服务端的套接字
	 void *requester = zmq_socket (context, ZMQ_REQ);
	zmq_connect (requester, "tcp://localhost:5555");
	
	 int request_nbr;
	 for (request_nbr = 0; request_nbr != 10; request_nbr++) 
	 {
		 char buffer [10];
		 printf ("正在发送 Hello %d...\n", request_nbr);
		//  zmq_send() 是一个高级的发送消息函数，它接受消息的内容和长度作为参数
		 zmq_send (requester, "Hello", 5, 0);
		 zmq_recv (requester, buffer, 10, 0);
		 printf ("接收到 World %d\n", request_nbr);
	}
	 zmq_close (requester);
	 zmq_ctx_destroy (context);
	 return 0;
 }
/*
使用REQ-REP套接字发送和接受消息是需要遵循一定规律的。客户端首先使用zmq_send()发送消息，再用zmq_recv()接收，
如此循环。如果打乱了这个顺序（如连续发送两次）则会报错。类似地，服务端必须先进行接收，后进行发送。

*/