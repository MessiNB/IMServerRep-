#include"UserManager.h"
#include "MysqlBusiness.h"
#include "muduo/base/Singleton.h"

using namespace muduo;

bool UserManager::init()
{
	// �����û���Ϣ
	if (loadUsers() == false)
	{
		cout << "load users failed" << endl;
		return false;
	}

	// ���� �û���ϵ
	for (auto it)
	{
	}

	Singleton<MysqlManager>::instance().Query();
}