#include "ClientSession.h"
#include <iostream>
#include "BinaryReader.h" 
using std::string;
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
	// TODO: 读出消息
	while (buf->readableBytes() >= sizeof(int32_t))  // 可能多条消息 作为一个包
	{
		int32_t packgeSize = 0;
		packgeSize = *static_cast<int32_t*> (buf->peek());  // 不真正读出来
		if (buf->readableBytes() < sizeof(int32_t) + packgeSize)  // 数据不完整
			return;
		buf->retrieve(sizeof(int32_t));	 // 读掉头部数据大小
		string msg;
		msg.assign(buf->peek(), packgeSize);   // 获取数据
		buf->retrieve(packgeSize)；
			// 处理消息
		if ( !this->process(conn,msg))
		{
			std::cout << "process error " << std::endl;
		}
	}

}

bool ClientSession::process(const muduo::net::TcpConnectionPtr& conn, string msg)
{
	BinaryReader reader(msg);
	int cmd = -1;
	if (!reader.readData<int>(cmd))
		return false;
	if (!reader.readData<int>(_seq))
		return false;
	size_t dataLen = 0;
	if (!reader.readData<size_t>(dataLen))
		return false;

	switch (cmd)
	{
	case 1000:  // 心跳

	default:
		break;
	}
	
	return true;
}
