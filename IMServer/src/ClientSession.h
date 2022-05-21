#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include "BinaryReader.h"
#include "BinaryWriter.h"
using namespace muduo;
using namespace muduo::net;
using namespace boost::uuids;

class ClientSession;

typedef std::shared_ptr<ClientSession>  ClientSessionPtr;

enum {
	MSG_TYPE_UNKNOW,
	//用户消息
	MSG_TYPE_HEARTBEAT = 1000,				//心跳
	MSG_TYPE_REGISTER ,							//注册
	MSG_TYPE_LOGIN,									// 连接
	MSG_TYPE_FRIENDLIST,							// 好友列表
	MSG_TYPE_FINDUSER,						// 
	MSG_TYPE_OPERATEFRIEND,					// 操作好友
	MSG_TYPE_USERSTATUS,						// 用户状态
	MSG_TYPE_UPDATEUSERINFO,				// 用户信息更改
	MSG_TYPE_MODIFYPWD,						// 修改密码
	MSG_TYPE_CREATEGROUP,					// 创建 群
	MSG_TYPE_GETGROUPMEMBERS,			// 获得群成员信息
	
	// 聊天消息
	MSG_TYPE_CHAT = 1100,						// 私聊
	MSG_TYPE_GROUPCHAT ,						// 群聊
};

//客户端会话，通过会话id 找到对应连接
class ClientSession
{
public:
	ClientSession(const TcpConnectionPtr& conn);

	//控制生命周期，防止提前销毁，或者二次销毁
	ClientSession(const ClientSession&) = delete;
	ClientSession& operator=(const ClientSession&) = delete;

	~ClientSession();

	//类型转换
	operator std::string()
	{
		return _sessionId;
	}

	// 发送数据
	void send(const muduo::net::TcpConnectionPtr& conn , BinaryWriter& writer)
	{
		string out = writer.getMsg();
		writer.clear();
		int len = out.size();
		writer.writeData(len);
		out = writer.getMsg + out;
		if (conn != NULL)
		{
			conn->send(out.c_str(), out.size());
		}
	}

	// 读事件
	void onRead(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp time);

	// 写事件
	void onSend(const std::string& buf);

	// 处理消息
	bool process(const muduo::net::TcpConnectionPtr& conn, string msg);

	// 心跳处理
	void onHeartbeatResponse(const muduo::net::TcpConnectionPtr& conn, const string &data);

	// 注册消息处理
	void onRegisterResponse(const muduo::net::TcpConnectionPtr& conn, const string& data);

private:
	//uuid_t _sessionId; //唯一 会话id（实际为 char[16]， 在Linux中）
	std::string _sessionId; //唯一 会话id （boost）
	int _seq = 0 ;  //会话seq 序号
};
