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

//ֻ�������ӣ������κ�ҵ��
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
	std::shared_ptr<muduo::net::TcpServer> _server;// TcpServer����ָ�

	std::map<std::string, ClientSessionPtr> _mapClientSession; //ͨ��ӳ�� session id �����ҵ� ��Ӧ����

	std::mutex _sessionLock;  // ���� _mapClientSessionӳ����Դ
};

typedef std::pair<std::string, ClientSessionPtr> ConnPair;