# CMakeList.txt: IMServer 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/src/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/src/lib)
message(STATUS "PROJECT_BINARY_DIR= ${PROJECT_BINARY_DIR}")
message(STATUS "PROJECT_SOURCE_DIR= ${PROJECT_SOURCE_DIR}")

set(muduo_base_srcs muduo/base/Timestamp.cc muduo/base/TimeZone.cc muduo/base/CountDownLatch.cc muduo/base/FileUtil.cc muduo/base/Logging.cc muduo/base/LogFile.cc muduo/base/LogStream.cc muduo/base/AsyncLogging.cc muduo/base/Thread.cc muduo/base/ProcessInfo.cc muduo/base/Condition.cc muduo/base/CurrentThread.cc muduo/base/Date.cc
)

set(muduo_net_srcs muduo/net/Acceptor.cc muduo/net/Buffer.cc muduo/net/Channel.cc muduo/net/Connector.cc muduo/net/Poller.cc muduo/net/poller/DefaultPoller.cc
muduo/net/poller/EPollPoller.cc muduo/net/poller/PollPoller.cc muduo/net/InetAddress.cc muduo/net/Socket.cc muduo/net/TcpClient.cc muduo/net/TcpConnection.cc muduo/net/TcpServer.cc muduo/net/EventLoop.cc
muduo/net/EventLoopThread.cc muduo/net/EventLoopThreadPool.cc muduo/net/Timer.cc muduo/net/TimerQueue.cc muduo/net/SocketsOps.cc
)

set(server_srcs IMServer.cpp main.cpp ClientSession.cpp)

set(mysql_srcs MysqlTool.cpp QueryResult.cpp Field.cpp MysqlBusiness.cpp)

set(json_srcs jsoncpp/json_reader.cpp jsoncpp/json_writer.cpp jsoncpp/json_value.cpp)

# 链接库文件目录(针对64位)
link_directories(
	${PROJECT_SOURCE_DIR} /lib
	/usr/lib/x86_64-linux-gnu/
)

# 头文件目录
include_directories(. ./muduo/base ./muduo/net)

#查找库是否在存在
find_library(MYSQL_LIB  /usr/lib/x86_64-linux-gnu/libmysqlclient.so)
if(NOT MYSQL_LIB)
	message(FATAL_ERROR "not found mysqlclient.so at  /usr/lib/x86_64-linux-gnu/")
endif(NOT MYSQL_LIB)

# 将源代码添加到此项目的可执行文件。
add_executable (IMServer ${server_srcs} ${muduo_base_srcs} ${muduo_net_srcs} ${mysql_srcs} ${json_srcs})

# 链接库
target_link_libraries(IMServer mysqlclient)

# TODO: 如有需要，请添加测试并安装目标。
