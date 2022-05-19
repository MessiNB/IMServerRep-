#include "ClientSession.h"
#include"jsoncpp/json.h"
#include <iostream>

using namespace std;
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
		buf->retrieve(packgeSize);
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
	if (!reader.readData<int>(cmd))  // 指令
		return false;	
	if (!reader.readData<int>(_seq))	 // 序列号
		return false;
	size_t dataLen = 0;
	if (!reader.readData<size_t>(dataLen)) // 数据段大小
		return false;
	string data;
	data.resize(dataLen);
	reader.readData(data);

	switch (cmd)
	{
	case MSG_TYPE_HEARTBEAT:  // 心跳
		onHeartbeatResponse(conn, data);
		break;
	case MSG_TYPE_REGISTER:
		break;
	case MSG_TYPE_LOGIN:
		break;
	case MSG_TYPE_FRIENDLIST:
		break;
	case MSG_TYPE_FINDUSER:
		break;
	case MSG_TYPE_OPERATEFRIEND:
		break;
	case MSG_TYPE_UPDATEUSERINFO:
		break;
	case MSG_TYPE_MODIFYPWD:
		break;
	case MSG_TYPE_CREATEGROUP:
		break;
	case MSG_TYPE_GETGROUPMEMBERS:
		break;
	case  MSG_TYPE_CHAT:
		break;
	case  MSG_TYPE_GROUPCHAT:
		break;
	default:
		break;
	}
	
	return true;
}

void ClientSession::onHeartbeatResponse(const muduo::net::TcpConnectionPtr& conn, const string& data)
{
	// 包的长度 4字节   不能压缩
	// 命令类型 4字节	   不能压缩
	// 包的序号 4 字节	不能压缩
	// 包的数据 数据长度（4字节）+ 包数据

	BinaryWriter writer;
	int cmd = MSG_TYPE_HEARTBEAT;
	writer.writeData<int>(cmd);   // 写入指令
	writer.writeData<int>(_seq);	// 写入序列号
	string empty;
	writer.writeData(empty);
	string out = writer.getMsg();  // 除了包长度没有的包
	writer.clear();

	size_t  data_len = out.size();  // 包长度(  没有int的包长度）
	writer.writeData(data_len);
	out = writer.getMsg() + out;  // 完整的包
	
	if (conn != NULL)
	{
		conn->send(out.c_str(), out.size());
	}
}


// 注册事件
void  ClientSession::onRegisterResponse(const muduo::net::TcpConnectionPtr& conn, const string& data)
{
	/*
		json 数据=   "username":"手机号" ，"nickname":"昵称" , "password":"密码"
	*/

	Json::Reader reader;
	Json::Value root;

	if (reader.parse(data, root) == false)
	{
		cout << "error json" << endl;
		return;
	}

	if (!root["username"].isString  || !root["nickname"].isString || root["password"].isString)
	{
		cout << "error type" << endl;
		return;
	}

}