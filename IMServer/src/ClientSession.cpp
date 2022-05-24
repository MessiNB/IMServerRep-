#include "ClientSession.h"
#include"jsoncpp/json.h"
#include <iostream>
#include "UserManager.h"
#include "muduo/base/Singleton.h"

using namespace std;
//有参构造
ClientSession::ClientSession(const TcpConnectionPtr& conn)
{
	// 生成uuid
	//uuid_generate(_sessionId); // 生成 id（Linux下）
	_sessionId = to_string(random_generator() ()); //注意是两个 （）
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
	while (buf->readableBytes() >= sizeof(int32_t))  // 可能多条消息 作为一个包 粘包
	{
		int32_t packgeSize = 0;
		packgeSize = *static_cast<int32_t*> (buf->peek());  // 不真正读出来
		if (buf->readableBytes() < sizeof(int32_t) + packgeSize)  // 数据不完整 （int型的数据大小 + 数据包大小）
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

// 消息处理
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

	// 判断格式 是否正确
	if (!root["username"].isString  || !root["nickname"].isString || root["password"].isString)
	{
		std::string res = this->getJsonString((int)RES_JOSNTYPE_ERROR, "json type error");
		writer.writeData(res);
		this->send(conn, writer);
		return;
	}

	// 获取数据
	User user;
	user._userName = root["username"].asString();
	user._nickName = root["nicknime"].asString();
	user._passWord = root["password"].asString();

	if (! Singleton<UserManager>::instance().addUser(user) )
	{	// 添加失败
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

// 登录事件处理
void ClientSession::onLoginResponse(const muduo::net::TcpConnectionPtr& conn, const string& data)
{
	// json 数据：{"user" :"手机号/UserId" ， "password" : "密码"，"status" : 1 }

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

	//json 格式有误
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

	// 获取 登录方式（手机号/ user id)
	if (value["user"].isString())
	{
		// 手机号
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
		// 未注册
		sring msg = this->getJsonString(RES_LOGIN_NOREGISTER, "user is not exist or password is incorrecr!");
		writer.writeData(msg);
		this->send(conn, writer);
		return;
	}

	if (user._passWord != password)
	{
		// 密码不正确
		sring msg = this->getJsonString(RES_LOGIN_NOPASSWORD, "user is not exist or password is incorrecr!");
		writer.writeData(msg);
		this->send(conn, writer);
		return;
	}

	// 登录成功 
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