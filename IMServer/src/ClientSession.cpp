#include "ClientSession.h"
#include <iostream>
#include "BinaryReader.h" 
using std::string;
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
		buf->retrieve(packgeSize)��
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
	if (!reader.readData<int>(cmd))
		return false;
	if (!reader.readData<int>(_seq))
		return false;
	size_t dataLen = 0;
	if (!reader.readData<size_t>(dataLen))
		return false;

	switch (cmd)
	{
	case 1000:  // ����

	default:
		break;
	}
	
	return true;
}
