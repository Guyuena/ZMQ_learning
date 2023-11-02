

# 版本和项目名称
cmake_minimum_required(VERSION 3.10)
project(example1)
# 设定编译标准为C++11
# 设定头文件路径和动态库路径
set(CXX_STANDARD 11)
set(INCLUDE_DIR "../tool/zmq425/include")
set(LIB_DIR "../tool/zmq425/bin/x64/Debug/v141/dynamic")
INCLUDE_DIRECTORIES(${INCLUDE_DIR})
LINK_DIRECTORIES(${LIB_DIR})
LINK_LIBRARIES(libzmq.lib)
# 生成可执行文件并连接到库文件
add_executable(publisher publisher.cpp)
add_executable(subscriber subscriber.cpp)
target_link_libraries(publisher libzmq.lib)
target_link_libraries(subscriber libzmq.lib)
