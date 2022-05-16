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

//�ͻ��˻Ự��ͨ���Ựid �ҵ���Ӧ����
class ClientSession
{
public:
	ClientSession(const TcpConnectionPtr& conn);

	//�����������ڣ���ֹ��ǰ���٣����߶�������
	ClientSession(const ClientSession&) = delete;
	ClientSession& operator=(const ClientSession&) = delete;

	~ClientSession();

	//����ת��
	operator std::string()
	{
		return _sessionId;
	}

	// ���¼�
	void onRead(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp time);

	// д�¼�
	void onSend(const std::string& buf);

private:
	//uuid_t _sessionId; //Ψһ �Ựid��ʵ��Ϊ char[16]�� ��Linux�У�
	std::string _sessionId; //Ψһ �Ựid ��boost��
};
