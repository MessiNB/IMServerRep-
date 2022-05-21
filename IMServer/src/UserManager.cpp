#include"UserManager.h"
#include "MysqlBusiness.h"
#include "muduo/base/Singleton.h"
using namespace std;
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
	for (auto& iter .: _cachedUsers)  // �����û���Ϣ �б�
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