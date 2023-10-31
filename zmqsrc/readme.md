## 源码编译：

//编译：gcc -o xxx xxx.c -lzmq

找不到zmq库，就需要带上编译参数   -lzmq

要生成调试信息，可以在编译时使用 `-g` 选项

gcc  -g -o xxx xxx.c -lzmq



## zmq

### `zmq_msg_t`

`zmq_msg_t` 是 ZeroMQ 库中的消息结构体，用于在 ZeroMQ 通信中表示消息数据。它主要用于在消息传递的上下文中创建、存储和传递消息。`zmq_msg_t` 结构体允许你在消息传递中更灵活地处理消息，包括消息的创建、销毁、拷贝和检索


1. **消息创建** ：你可以使用 `zmq_msg_init` 函数创建一个空的 `zmq_msg_t` 结构，然后使用 `zmq_msg_init_size` 函数初始化一个具有指定大小的消息。
2. **消息数据设置** ：使用 `zmq_msg_data` 函数可以获取消息的数据指针，以便将数据填充到消息中。
3. **消息长度获取** ：通过 `zmq_msg_size` 函数可以获取消息的有效数据长度。
4. **消息拷贝** ：你可以使用 `zmq_msg_copy` 函数将一个消息的内容复制到另一个消息中，以支持消息的复制和传递。
5. **消息销毁** ：使用 `zmq_msg_close` 函数可以销毁消息并释放相关资源。这在消息不再需要时非常重要，以避免内存泄漏。
6. **消息初始化** ：`zmq_msg_init` 和 `zmq_msg_init_size` 函数用于初始化消息，这是消息处理的起点。
7. **消息的发送和接收** ：`zmq_msg_t` 结构体通常与 ZeroMQ 套接字（socket）一起使用，可以通过套接字发送和接收消息。


### zmq_msg_init_data、zmq_msg_init

`zmq_msg_init_data` 和 `zmq_msg_init` 是 ZeroMQ 库中用于初始化消息结构体 `zmq_msg_t` 的两种不同方法。它们的区别在于如何提供消息的初始数据：

1. **zmq_msg_init_data**:

   - `zmq_msg_init_data` 函数初始化一个消息，并允许你提供一个指向消息数据的指针。这意味着你可以直接指定消息的初始数据。
   - 使用 `zmq_msg_init_data` 时，你需要指定初始数据的指针、数据的长度以及一个可选的销毁回调函数。销毁回调函数用于在消息不再需要时清理数据。
2. **zmq_msg_init**:

   - `zmq_msg_init` 函数初始化一个消息，但不要求你提供初始数据。你可以稍后使用 `zmq_msg_data` 来获取消息的数据指针，然后填充消息的内容。
   - `zmq_msg_init` 通常用于创建一个空的消息，然后将数据逐步添加到消息中，以满足具体的通信需求。

基本上，`zmq_msg_init_data` 更适合在初始化消息时知道要发送的具体数据，而 `zmq_msg_init` 更适合在稍后才知道要发送的数据的情况。两者的选择取决于你的通信模式和数据的可用性。

下面是它们的基本用法示例：

使用 `zmq_msg_init_data` 初始化消息：

```cpp
zmq_msg_t msg;
const char *data = "Hello, ZeroMQ!";
zmq_msg_init_data(&msg, (void *)data, strlen(data), NULL, NULL);
```

使用 `zmq_msg_init` 初始化空消息，然后添加数据：

```cpp
zmq_msg_t msg;
zmq_msg_init(&msg);
const char *data = "Hello, ZeroMQ!";
memcpy(zmq_msg_data(&msg), data, strlen(data));
zmq_msg_close(&msg); // 不要忘记销毁消息
```

在实际应用中，你可以根据情况选择使用哪种方法，以满足你的消息传递需求。
