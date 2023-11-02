一个系统可以同时充当服务器和客户端。 也就是说，一个进程充当服务器，另一个进程充当客户端。 这也可能发生在客户端和服务器进程驻留在同一台机器上。

![img](https://w3schools.cn/computer_network/images/client_server.jpg)







# 套接字

套接字是网络编程中的一种通信机制，是支持TCP/IP的网络通信的基本操作单元，可以看做是不同主机之间的进程进行双向通信的端点，简单的说就是**通信的两方的一种约定，用套接字中的相关函数来完成通信过程**。



![img](https://pic3.zhimg.com/v2-19c8b65ebd6c4c888f7dba6e5914f55a_r.jpg)



![image-20231003215712437](C:\Users\jc\AppData\Roaming\Typora\typora-user-images\image-20231003215712437.png)



但在ZMQ的世界里，套接字是智能的、多线程的，能够自动地维护一组完整的连接。你无法看到它们，甚至不能直接操纵这些连接。当你进行消息的收发、轮询等操作时，只能和ZMQ套接字打交道，而不是连接本身。所以说，ZMQ世界里的连接是私有的，不对外部开放，这也是ZMQ易于扩展的原因之一。



## zmq使用套接字传递数据

发送和接收消息使用的是zmq_send()和zmq_recv()这两个函数。虽然函数名称看起来很直白，但由于ZMQ的I/O模式和传统的TCP协议有很大不同，因此还是需要花点时间去理解的。

![1](https://github.com/anjuke/zguide-cn/raw/master/images/chapter2_1.png)

让我们看一看TCP套接字和ZMQ套接字之间在传输数据方面的区别：

* ZMQ套接字传输的是消息，而不是字节（TCP）或帧（UDP）。消息指的是一段指定长度的二进制数据块，我们下文会讲到消息，这种设计是为了性能优化而考虑的，所以可能会比较难以理解。
* ZMQ套接字在后台进行I/O操作，也就是说无论是接收还是发送消息，它都会先传送到一个本地的缓冲队列，这个内存队列的大小是可以配置的。
* ZMQ套接字可以和多个套接字进行连接（如果套接字类型允许的话）。TCP协议只能进行点对点的连接，而ZMQ则可以进行一对多（类似于无线广播）、多对多（类似于邮局）、多对一（类似于信箱），当然也包括一对一的情况。
* ZMQ套接字可以发送消息给多个端点（扇出模型），或从多个端点中接收消息（扇入模型）

![2](https://github.com/anjuke/zguide-cn/raw/master/images/chapter2_2.png)

所以，向套接字写入一个消息时可能会将消息发送给很多节点，相应的，套接字又会从所有已建立的连接中接收消息。zmq_recv()方法使用了公平队列的算法来决定接收哪个连接的消息。

调用zmq_send()方法时其实并没有真正将消息发送给套接字连接。消息会在一个内存队列中保存下来，并由后台的I/O线程异步地进行发送。如果不出意外情况，这一行为是非阻塞的。所以说，即便zmq_send()有返回值，并不能代表消息已经发送。当你在用zmq_msg_init_data()初始化消息后，你不能重用或是释放这条消息，否则ZMQ的I/O线程会认为它在传输垃圾数据。这对初学者来讲是一个常犯的错误，下文我们会讲述如何正确地处理消息。



## zmq消息的使用方法

ZMQ的传输单位是消息，即一个二进制块。你可以使用任意的序列化工具，如谷歌的Protocal Buffers、XDR、JSON等，将内容转化成ZMQ消息。不过这种转化工具最好是便捷和快速的，这个请自己衡量。

在内存中，ZMQ消息由zmq_msg_t结构表示（每种语言有特定的表示）。在C语言中使用ZMQ消息时需要注意以下几点：

* 你需要创建和传递zmq_msg_t对象，而不是一组数据块；
* 读取消息时，先用zmq_msg_init()初始化一个空消息，再将其传递给zmq_recv()函数；
* 写入消息时，先用zmq_msg_init_size()来创建消息（同时也已初始化了一块内存区域），然后用memcpy()函数将信息拷贝到该对象中，最后传给zmq_send()函数；
* 释放消息（并不是销毁）时，使用zmq_msg_close()函数，它会将对消息对象的引用删除，最终由ZMQ将消息销毁；
* 获取消息内容时需使用zmq_msg_data()函数；若想知道消息的长度，可以使用zmq_msg_size()函数；
* 至于zmq_msg_move()、zmq_msg_copy()、zmq_msg_init_data()函数，在充分理解手册中的说明之前，建议不好贸然使用。

以下是一段处理消息的典型代码，如果之前的代码你有看的话，那应该会感到熟悉。这段代码其实是从zhelpers.h文件中抽出的

















### TCP  socket API：

```cpp
//创建socket文件描述符  (TCP/UDP,客户端+服务器)

int socket(int domain, int type, int protocol);
```

**参数1(domain):** 选择创建的套接字所用的协议族；
AF_INET ： IPv4协议；
AF_INET6： IPv6协议；
AF_LOCAL: Unix域协议；
AF_ROUTE：路由套接口；
AF_KEY ：密钥套接口。
**参数2(type)：**指定套接口类型，所选类型有：
SOCK_STREAM：字节流套接字；
SOCK_DGRAM : 数据报套接字;
SOCK_RAW : 原始套接口。
procotol: 使用的特定协议，一般使用默认协议（NULL）。

```cpp
//绑定端口号  （TCP/IP，服务器）

int bind(int socket, const struct sockaddr *address, socklen_t address_len);
```

**参数1(socket) :** 是由socket()调用返回的并且未作连接的套接字描述符（套接字号）。
**参数2(address)：**指向特定协议的地址指针。
**参数3(address_len)：**上面地址结构的长度。
**返回值：**没有错误，bind()返回0，否则SOCKET_ERROR。

```cpp
//开始监听socket  (TCP,服务器）

int listen(int socket, int backlog);
```

**参数1(sockfd)：**是由socket()调用返回的并且未作连接的套接字描述符（套接字号）。
**参数2(backlog)：**所监听的端口队列大小。

```cpp
//接受请求  （TCP，服务器）

int accept(int socket, struct sockaddr* address, socklen_t* address_len);
```

**参数1(socket) :** 是由socket()调用返回的并且未作连接的套接字描述符（套接字号）。
**参数2(address)：**指向特定协议的地址指针。
**参数3(addrlen)：**上面地址结构的长度。
**返回值：**没有错误，bind()返回0，否则SOCKET_ERROR。

```cpp
//建立连接  （TCP，客户端）

int connect(int sockfd, const struct struct sockaddr *addr, aocklen_t addrlen);
//关闭套接字
int close(int fd);
```

**参数(fd)：**是由socket()调用返回的并且未作连接的套接字描述符（套接字号）。
socket API是一层抽象的网络编程接口，适用于各种底层网络协议，如IPv4，IPv6，……



![img](https://pic2.zhimg.com/v2-2a4a056a2d569fbe5b9bdf44b34510d5_r.jpg)











# 发布-订阅模式







![img](https://pic2.zhimg.com/v2-391ffa2847eb738306cd192d52d7c5e1_r.jpg)

![image-20231003220036330](C:\Users\jc\AppData\Roaming\Typora\typora-user-images\image-20231003220036330.png)





![image-20231003220134530](C:\Users\jc\AppData\Roaming\Typora\typora-user-images\image-20231003220134530.png)







# 分布式处理





下面一个示例程序中，我们将使用ZMQ进行超级计算，也就是并行处理模型：

- 任务分发器会生成大量可以并行计算的任务；
- 有一组worker会处理这些任务；
- 结果收集器会在末端接收所有worker的处理结果，进行汇总。

现实中，worker可能散落在不同的计算机中，利用GPU（图像处理单元）进行复杂计算。下面是任务分发器的代码，它会生成100个任务，任务内容是让收到的worker延迟若干毫秒。整体系统架构如下图所示



![img](https://pic1.zhimg.com/v2-b5f0db269480aceb6590007f8ad9dfe8_r.jpg)





## PUSH 推   **数据生产者** 

- 推方式的**主动权在数据源系统侧（**即推送数据的程序运行在数据源系统侧**）**，数据源方根据自己数据产生的方式、频率以及数据量来**采用一个适合数据源系统的方式**将数据推送到后台处理系统
- 数据生产者决定何时向消费者推送数据。数据消费者不知道何时会收到数据更新。



![image-20231003222140763](C:\Users\jc\AppData\Roaming\Typora\typora-user-images\image-20231003222140763.png)





## PULL  拉     **数据消费者**

- 该方式的**主动权则掌握在数据处理后台这边（**即数据接入程序部署在后台系统这边**）**，对数据获取的频率、数据量和获取方式完全是后台端决定，这种方式是**对业务处理最为友好的方式**，因为你数据拉取的行为(数据量、拉取频率、拉取方法)可以完全适配你后续的数据处理频率，且可以做到数据接入层与数据计算处理层的无缝衔接，对业务需求非常友好。
- 数据消费者决定自己何时请求并接收数据；数据持有者只能被动地响应请求。



![image-20231003222226226](C:\Users\jc\AppData\Roaming\Typora\typora-user-images\image-20231003222226226.png)





# ZMQ接口



[ZeroMQ(ZMQ)函数接口英汉直译 - fengbohello - 博客园 (cnblogs.com)](https://www.cnblogs.com/fengbohello/p/4230135.html)

