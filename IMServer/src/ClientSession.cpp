#include "ClientSession.h"
#include"jsoncpp/json.h"
#include <iostream>
#include "UserManager.h"
#include "muduo/base/Singleton.h"

using namespace std;
//�вι���
ClientSession::ClientSession(const TcpConnectionPtr& conn)
{
	// ����uuid
	//uuid_generate(_sessionId); // ���� id��Linux�£�
	_sessionId = to_string(random_generator() ()); //ע�������� ����
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
	while (buf->readableBytes() >= sizeof(int32_t))  // ���ܶ�����Ϣ ��Ϊһ���� ճ��
	{
		int32_t packgeSize = 0;
		packgeSize = *static_cast<int32_t*> (buf->peek());  // ������������
		if (buf->readableBytes() < sizeof(int32_t) + packgeSize)  // ���ݲ����� ��int�͵����ݴ�С + ���ݰ���С��
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

// ��Ϣ����
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
		onRegisterResponse(conn, data);
		break;
	case MSG_TYPE_LOGIN:
		onLoginResponse(conn, data);
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
	BinaryWriter writer;
	writer.writeData((int)MSG_TYPE_REGISTER);
	writer.writeData(this->_seq);
	if (reader.parse(data, root) == false)
	{
		std::string res = this->getJsonString((int)RES_PARSE_FAILED, "json parse faied");
		writer.writeData(res);
		this->send(conn, writer);
		return;
	}

	// �жϸ�ʽ �Ƿ���ȷ
	if (!root["username"].isString  || !root["nickname"].isString || root["password"].isString)
	{
		std::string res = this->getJsonString((int)RES_JOSNTYPE_ERROR, "json type error");
		writer.writeData(res);
		this->send(conn, writer);
		return;
	}

	// ��ȡ����
	User user;
	user._userName = root["username"].asString();
	user._nickName = root["nicknime"].asString();
	user._passWord = root["password"].asString();

	if (! Singleton<UserManager>::instance().addUser(user) )
	{	// ���ʧ��
		cout << "add user failed" << endl;
		std::string res =  this->getJsonString((int)RES_REGISTER_FAIED, "register failed");
		writer.writeData(res);
	}
	else
	{
		std::string res = this->getJsonString((int)RES_REGISTER_SUCCESS, "register success");
		writer.writeData(res);
	}
	this->send(conn, writer);
}

// ��¼�¼�����
void ClientSession::onLoginResponse(const muduo::net::TcpConnectionPtr& conn, const string& data)
{
	// json ���ݣ�{"user" :"�ֻ���/UserId" �� "password" : "����"��"status" : 1 }

	Json::Reader reader;
	Json::Value value;
	BinaryWriter  writer;

	writer.writeData((int)MSG_TYPE_LOGIN);
	writer.writeData(this->_seq);
	if (reader.parse(data, value) == false)
	{
		std::string res = this->getJsonString(RES_PARSE_FAILED, "json parse faied");
		writer.writeData(res);
		this->send(conn, writer);
		return;
	}

	//json ��ʽ����
	if (!value["password"].isString() || !value["stastus"].isInt())
	{
		std::string res = this->getJsonString(RES_JOSNTYPE_ERROR, "json josn type error");
		writer.writeData(res);
		this->send(conn, writer);
		return;
	}
	User user;
	string sAccount;
	int32_t iAccount = -1;
	bool bRet = false;
	string password = value["password"].asString();
	int status = value["status"].asInt();

	// ��ȡ ��¼��ʽ���ֻ���/ user id)
	if (value["user"].isString())
	{
		// �ֻ���
		sAccount = value["user"].asString();
		bRet = Singleton<UserManager>::instance().getUserInfoFromCached(sAccount, user);
		if (!bRet)
			bRet = Singleton<UserManager>::instance().getUserInfoFromDB(sAccount, user);
	}
	else
	{
		iAccount = value["user"].asInt();
		bRet = Singleton<UserManager>::instance().getUserInfoFromCached(iAccount, user);
		if (!bRet)
		{
			bRet = Singleton<UserManager>::instance().getUserInfoFromDB(iAccount, user);
		}
	}

	if (!bRet)
	{
		// δע��
		sring msg = this->getJsonString(RES_LOGIN_NOREGISTER, "user is not exist or password is incorrecr!");
		writer.writeData(msg);
		this->send(conn, writer);
		return;
	}

	if (user._passWord != password)
	{
		// ���벻��ȷ
		sring msg = this->getJsonString(RES_LOGIN_NOPASSWORD, "user is not exist or password is incorrecr!");
		writer.writeData(msg);
		this->send(conn, writer);
		return;
	}

	// ��¼�ɹ� 
	Json::Value response;
	response["code"] = (int)RES_LOGIN_SUCCESS;
	response["msg"] = "user is not exist or password is incorrecr!";
	response["userid"] = user._userId;
	response["username"] = user._userName;
	response["nickname"] = user._nickName;
	response["facetype"] = user._faceType;
	response["customface"] = user._customFace;
	response["gender"] = user._gender;
	response["birthday"] = user._birthDay;
	response["_signature"] = user._signature;
	response["_adress"] = user._adress;
	response["_pthone"] = user._pthone;
	response["_mail"] = user._mail;
	
	string msg = response.toStyledString();
	writer.writeData(msg);
	this->send(conn, writer);
}