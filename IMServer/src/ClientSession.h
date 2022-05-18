#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
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
	MSG_TYPE_LOGIN,									//
	MSG_TYPE_FRIENDLIST,
	MSG_TYPE_FRIENDUSER,
	MSG_TYPE_OPERATEFRIEND,
	MSG_TYPE_USERSTATUS,
	MSG_TYPE_UPDATEUSERINFO,
	MSG_TYPE_MODIFYPWD,
	MSG_TYPE_CREATEGROUP,
	MSG_TYPE_GETGROUPMEMBERS,
	
	// 聊天消息
	MSG_TYPE_CHAT = 1100,
	MSG_TYPE_GROUPCHAT ,
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

	// 读事件
	void onRead(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp time);

	// 写事件
	void onSend(const std::string& buf);

	// 处理消息
	bool process(const muduo::net::TcpConnectionPtr& conn, string msg);

private:
	//uuid_t _sessionId; //唯一 会话id（实际为 char[16]， 在Linux中）
	std::string _sessionId; //唯一 会话id （boost）
	int _seq = 0 ;  //会话seq 序号
};
