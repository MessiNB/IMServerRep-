#include <mutex>
#include <memory>
#include <list>
#include <string>
#include <iostream>
#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"
#include "ClientSession.h"


using namespace muduo;
using namespace muduo::net;

//只负责连接（不做任何业务）
class IMServer {
public:
	IMServer() = default;
	IMServer& operator=(const IMServer&) = delete;
	IMServer(const IMServer&) = delete;
	~IMServer()=default ;
	bool init(const std::string& ip, short port, EventLoop* loop);

protected:
	void OnConnection(const muduo::net::TcpConnectionPtr& conn);
	
	void OnClose(const muduo::net::TcpConnectionPtr& conn);

private:
	std::shared_ptr<muduo::net::TcpServer> _server;// TcpServer共享指針

	std::map<std::string, ClientSessionPtr> _mapClientSession; //通过映射 session id 快速找到 对应连接

	std::mutex _sessionLock;  // 控制 _mapClientSession映射资源
};

typedef std::pair<std::string, ClientSessionPtr> ConnPair;