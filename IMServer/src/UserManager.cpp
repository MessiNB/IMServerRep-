#include"UserManager.h"
#include "MysqlBusiness.h"
#include "muduo/base/Singleton.h"
#include<sstream>
using namespace std;
using namespace muduo;

bool UserManager::init()
{
	// 加载用户信息
	if (loadUsers() == false)
	{
		cout << "load users failed" << endl;
		return false;
	}

	// 加载 用户关系 
	for (auto& iter : _cachedUsers)  // 遍历用户信息 列表
	{
		if (!loadRelation(iter._userId, iter._friends))
		{
			cout << "loadRelation error " << endl;
		}
		
	}
}

// 缓存加载 用户信息 
bool UserManager::getUserInfoFromCached(const string& username, User& user)
{
	// TODO：登录需要加锁嘛？
	for (const auto& it : _cachedUsers)
	{
		if (it._userName == username)
		{
			user = it;
			return true;
		}
	}
	return false;
}

bool UserManager::getUserInfoFromCached(const int userid, User& user)
{
	for (const auto& it : _cachedUsers)
	{
		if (it._userId == userid)
		{
			user = it;
			return true;
		}
	}
	return false;
}

// 数据库加载用户信息
bool UserManager::getUserInfoFromDB(const std::string& username, User& user)
{
	return true;
}

bool UserManager::getUserInfoFromDB(const int userid, User& user)
{
	return true;
}

bool UserManager::addUser(User& user)
{
	stringstream sql;
	sql << "INSERT INTO user(user_id,username,nickname,password,register_time)" <<
		"VALUES(" << _currentUserId+1 << ",'" << user._userName << "','" <<
		user._nickName << "','" << user._nickName<< "',NOW())";
	bool result = Singleton<MysqlManager>::instance().excute(sql.str());
	if (result == false)
		return false;
	_currentUserId++;
	user._userId = _currentUserId;
	user._faceType = 0; // 默认使用默认头像
	user._birthDay = 20000101;
	user._gender = 0;   // 默认 女
	user._ownerid = 0;
	{
		std::lock_guard<mutex> guard(_mutex);
		_cachedUsers.push_back(user);
	}
	return true;
}

// 记载用户信息
bool UserManager::loadUsers()
{
	stringstream sql;
	sql << "SELECT user_id, username, nickname, password,  facetype, customface, gender, birthday, signature, address, phonenumber, mail FROM user ORDER BY  user_id DESC";  //  降序加载

	QueryResultPtr  res = Singleton<MysqlManager> ::instance().Query(sql.str());

	if (NULL == res)
		return false;
	while(res->nextRow())
	{
		Field* row = res->Fetch();
		User user;
		user._userId = row[0].toInt32();
		if (user._userId > _currentUserId)
			_currentUserId = user._userId;	//更新当前最大 user_id;
		user._userName = row[1].getValue();
		user._nickName = row[2].getValue();
		user._passWord = row[3].getValue();
		user._faceType = row[4].toInt32();
		user._customFace = row[5].getValue();
		user._gender = row[6].toInt32();
		user._birthDay = row[7].toInt32();
		user._signature = row[8].getValue();
		user._adress = row[9].getValue();
		user._pthone = row[10].getValue();
		user._mail = row[11].getValue();
		_cachedUsers.push_back(user);
	}
	return true;
}

bool UserManager::loadRelation(int32_t userid,  set<int32_t>& friends)
{
	stringstream sql;
	sql << "SELECT user_id1, user_id2 FROM user_relationship WHERE user_id1=" << userid << " OR user_id2=" << userid << " ;";
	QueryResultPtr res = Singleton<MysqlManager>::instance().Query(sql.str());
	if (res == nullptr)
		return false;
	while (res->nextRow())
	{
		Field* row = res->Fetch();
		int id1 = row[0].toInt32();
		int id2 = row[1].toInt32();
		if (id1 == userid)
			friends.insert(id2);
		else
			friends.insert(id1);
	}
	return true;
}
