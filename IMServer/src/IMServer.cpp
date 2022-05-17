#include "IMServer.h"


// 初始化服務器
bool IMServer::init(const std::string& ip, short port, EventLoop* loop) {
	// 绑定地址，设置端口服用
	InetAddress addr(ip, port);
	_server.reset(new TcpServer(loop, addr, "IMServer", TcpServer::kNoReusePort));
	
	// 注册 连接事件回调函数
	_server->setConnectionCallback(std::bind(&IMServer::OnConnection, this, std::placeholders::_1));

	// 开始运行
	_server->start();

	return true;
	
}

// 处理连接事件
void IMServer::OnConnection(const muduo::net::TcpConnectionPtr& conn) {

	if (conn->connected ( ) )
	{
		// 创建 session 并添加至 映射
		std::lock_guard<std::mutex> guard(_sessionLock);
		ClientSessionPtr client(new ClientSession(conn));
		_mapClientSession.insert(ConnPair((std::string)*client, client));
	}
	else
	{
		// 当我们来出来连接事件时， 可能连接已经断开
		OnClose(conn);
	}
	
}

// 处理连接中断
void IMServer::OnClose(const muduo::net::TcpConnectionPtr& conn) {
	//TODO: 找到这个连接，进行关闭
	auto iter = _mapClientSession.find(conn->name());
	if (iter != _mapClientSession.end())
	{
		//关闭连接
		_mapClientSession.erase(iter);
	}
	else
	{
		// 找不到该连接
		std::cout << "can't find conn.name:  " << conn->name() << std::endl;
		
	}
}