#include"UserManager.h"
#include "MysqlBusiness.h"
#include "muduo/base/Singleton.h"
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
	for (auto& iter .: _cachedUsers)  // 遍历用户信息 列表
	{
		if (!loadReles(iter._userId, iter._friends))
		{
			cout << "loadReles error " << endl;
		}
		
	}

	Singleton<MysqlManager>::instance().Query();
}

bool UserManager::addUser(User& user)
{
	
}

bool UserManager::loadUsers()
{
	stringstream sql;
	if(Singleton<MysqlManager>::instance().Query(sql.str()) == false)
	{
		
	}
}