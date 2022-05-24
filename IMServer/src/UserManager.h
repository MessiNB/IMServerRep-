#pragma once
#include<uinstd.h>
#include<stdio.h>
#include<sys/types.h>
#include <errno.h>
#include"base/Logging.h"
#include"base/Singleton"
#include<string>
#include<set>
#include <list>
#include<mutex>

class User
{
public:
	USer() = default;
	~USer() = default;
	int32_t				 _userId = 0; //int32_t
	std::string			 _userName;
	std::string			 _passWord;
	std::string			 _nickName;
	std::string			_signature; //个性签名
	int32_t				_faceType = -1;  // 默认头像
	std::string			_customFace;
	std::string			 _customFaceFmt; // 自定义头像格式
	int32_t				_gender = 0;  // 性别 （默认女性）
	int32_t				 _birthDay;  // 生日
	std::string			 _adress;
	std::string			 _pthone;
	std::string			 _mail;
	int32_t				 _ownerid ;	// 用于标记群主信息

	set<int32_t>	 _friends;  // 好友id

};


class UserManager
{
public:
	UserManager();
	~UserManager();

	bool init();

	UserManager(const UserManager&) = delete;
	UserManager& operator=(const UserManager&) = delete;


	// 添加用户
	bool addUser(User& user);

	// 从数据库 加载所有用户信息、
	bool loadUsers();


	// 加载用户关系
	bool loadRelation(int32_t userId, std::set<int32_t>& friends);

	// 从缓存获取单个用户信息
	bool getUserInfoFromCached(const std::string& username, User& user);
	bool getUserInfoFromCached(const int userid, User& user);

	// 从 数据库 获取 单个用户信息
	bool getUserInfoFromDB(const std::string& username, User& user);
	bool getUserInfoFromDB(const int userid, User& user);

private:
	std::list<User> _cachedUsers;  // 用户信息缓存
	int32_t _currentUserId = 10000; //当前可用 用户id
	std::mutex _mutex;     // 缓存互斥体
};

