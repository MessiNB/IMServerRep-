#include "ClientSession.h"
#include"jsoncpp/json.h"
#include <iostream>

using namespace std;
//�вι���
ClientSession::ClientSession(const TcpConnectionPtr& conn)
{
	// ����uuid
	//uuid_generate(_sessionId); // ���� id��Linux�£�
	_sessionId = to_string(random_generator() ());
	TcpConnectionPtr* client = const_cast<TcpConnectionPtr*> (&conn);
	*const_cast<std::string*> (&conn->name()) = _sessionId; //�޸����ӵ�name

	// ����Ϣ�¼�
	(*client)->setMessageCallback(std::bind(&ClientSession::onRead, this, std::placeholders::_1, _2, _3));


}

ClientSession::~ClientSession() {

}

void ClientSession::onRead(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp time)
{
	// TODO: ������Ϣ
	while (buf->readableBytes() >= sizeof(int32_t))  // ���ܶ�����Ϣ ��Ϊһ����
	{
		int32_t packgeSize = 0;
		packgeSize = *static_cast<int32_t*> (buf->peek());  // ������������
		if (buf->readableBytes() < sizeof(int32_t) + packgeSize)  // ���ݲ�����
			return;
		buf->retrieve(sizeof(int32_t));	 // ����ͷ�����ݴ�С
		string msg;
		msg.assign(buf->peek(), packgeSize);   // ��ȡ����
		buf->retrieve(packgeSize);
			// ������Ϣ
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
	if (!reader.readData<int>(cmd))  // ָ��
		return false;	
	if (!reader.readData<int>(_seq))	 // ���к�
		return false;
	size_t dataLen = 0;
	if (!reader.readData<size_t>(dataLen)) // ���ݶδ�С
		return false;
	string data;
	data.resize(dataLen);
	reader.readData(data);

	switch (cmd)
	{
	case MSG_TYPE_HEARTBEAT:  // ����
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
	// ���ĳ��� 4�ֽ�   ����ѹ��
	// �������� 4�ֽ�	   ����ѹ��
	// ������� 4 �ֽ�	����ѹ��
	// �������� ���ݳ��ȣ�4�ֽڣ�+ ������

	BinaryWriter writer;
	int cmd = MSG_TYPE_HEARTBEAT;
	writer.writeData<int>(cmd);   // д��ָ��
	writer.writeData<int>(_seq);	// д�����к�
	string empty;
	writer.writeData(empty);
	string out = writer.getMsg();  // ���˰�����û�еİ�
	writer.clear();

	size_t  data_len = out.size();  // ������(  û��int�İ����ȣ�
	writer.writeData(data_len);
	out = writer.getMsg() + out;  // �����İ�
	
	if (conn != NULL)
	{
		conn->send(out.c_str(), out.size());
	}
}


// ע���¼�
void  ClientSession::onRegisterResponse(const muduo::net::TcpConnectionPtr& conn, const string& data)
{
	/*
		json ����=   "username":"�ֻ���" ��"nickname":"�ǳ�" , "password":"����"
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