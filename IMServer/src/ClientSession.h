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
	//�û���Ϣ
	MSG_TYPE_HEARTBEAT = 1000,				//����
	MSG_TYPE_REGISTER ,							//ע��
	MSG_TYPE_LOGIN,									// ����
	MSG_TYPE_FRIENDLIST,							// �����б�
	MSG_TYPE_FINDUSER,						// 
	MSG_TYPE_OPERATEFRIEND,					// ��������
	MSG_TYPE_USERSTATUS,						// �û�״̬
	MSG_TYPE_UPDATEUSERINFO,				// �û���Ϣ����
	MSG_TYPE_MODIFYPWD,						// �޸�����
	MSG_TYPE_CREATEGROUP,					// ���� Ⱥ
	MSG_TYPE_GETGROUPMEMBERS,			// ���Ⱥ��Ա��Ϣ
	
	// ������Ϣ
	MSG_TYPE_CHAT = 1100,						// ˽��
	MSG_TYPE_GROUPCHAT ,						// Ⱥ��
};

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

	// ��������
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

	// ���¼�
	void onRead(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp time);

	// д�¼�
	void onSend(const std::string& buf);

	// ������Ϣ
	bool process(const muduo::net::TcpConnectionPtr& conn, string msg);

	// ��������
	void onHeartbeatResponse(const muduo::net::TcpConnectionPtr& conn, const string &data);

	// ע����Ϣ����
	void onRegisterResponse(const muduo::net::TcpConnectionPtr& conn, const string& data);

private:
	//uuid_t _sessionId; //Ψһ �Ựid��ʵ��Ϊ char[16]�� ��Linux�У�
	std::string _sessionId; //Ψһ �Ựid ��boost��
	int _seq = 0 ;  //�Ựseq ���
};
