#include "ClientSession.h"

//有参构造
ClientSession::ClientSession(const TcpConnectionPtr& conn)
{
	// 生成uuid
	//uuid_generate(_sessionId); // 生成 id（Linux下）
	_sessionId = to_string(random_generator() ());
	TcpConnectionPtr* client = const_cast<TcpConnectionPtr*> (&conn);
	*const_cast<std::string*> (&conn->name()) = _sessionId; //修改连接的name

	// 绑定消息事件
	(*client)->setMessageCallback(std::bind(&ClientSession::onRead, this, std::placeholders::_1, _2, _3));
	

}

ClientSession::~ClientSession() {

}

void ClientSession::onRead(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp time)
{

}