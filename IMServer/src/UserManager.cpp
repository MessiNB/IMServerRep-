#include"UserManager.h"
#include "MysqlBusiness.h"
#include "muduo/base/Singleton.h"

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
	for (auto it)
	{
	}

	Singleton<MysqlManager>::instance().Query();
}