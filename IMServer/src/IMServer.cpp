#include "IMServer.h"


// ��ʼ��������
bool IMServer::init(const std::string& ip, short port, EventLoop* loop) {
	// �󶨵�ַ�����ö˿ڷ���
	InetAddress addr(ip, port);
	_server.reset(new TcpServer(loop, addr, "IMServer", TcpServer::kNoReusePort));
	
	// ע�� �����¼��ص�����
	_server->setConnectionCallback(std::bind(&IMServer::OnConnection, this, std::placeholders::_1));

	// ��ʼ����
	_server->start();

	return true;
	
}

// ���������¼�
void IMServer::OnConnection(const muduo::net::TcpConnectionPtr& conn) {

	if (conn->connected ( ) )
	{
		// ���� session ������� ӳ��
		std::lock_guard<std::mutex> guard(_sessionLock);
		ClientSessionPtr client(new ClientSession(conn));
		_mapClientSession.insert(ConnPair((std::string)*client, client));
	}
	else
	{
		// �����������������¼�ʱ�� ���������Ѿ��Ͽ�
		Onclose(conn);
	}
	
}

// ���������ж�
void IMServer::Onclose(const muduo::net::TcpConnectionPtr& conn) {
	//TODO: �ҵ�������ӣ����йر�
	auto iter = _mapClientSession.find(conn->name());
	if (iter != _mapClientSession.end())
	{
		//�ر�����
		_mapClientSession.erase(iter);
	}
	else
	{
		// �Ҳ���������
		std::cout << "can't find conn.name:  " << conn->name() << std::endl;
		
	}
}