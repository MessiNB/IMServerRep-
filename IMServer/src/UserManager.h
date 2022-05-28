#pragma once
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include <errno.h>
#include"muduo/base/Logging.h"
#include"muduo/base/Singleton.h"
#include<string>
#include<set>
#include <list>
#include<mutex>

class User
{
public:
	User() = default;
	~User() = default;
	int32_t				 _userId = 0; //int32_t
	std::string			 _userName;
	std::string			 _passWord;
	std::string			 _nickName;
	std::string			_signature; //����ǩ��
	int32_t				_faceType = -1;  // Ĭ��ͷ��
	std::string			_customFace;
	std::string			 _customFaceFmt; // �Զ���ͷ���ʽ
	int32_t				_gender = 0;  // �Ա� ��Ĭ��Ů�ԣ�
	int32_t				 _birthDay;  // ����
	std::string			 _adress;
	std::string			 _pthone;
	std::string			 _mail;
	int32_t				 _ownerid ;	// ���ڱ��Ⱥ����Ϣ

	std::set<int32_t>	 _friends;  // ����id

};


class UserManager
{
public:
	UserManager()=default;
	~UserManager()=default;

	bool init();

	UserManager(const UserManager&) = delete;
	UserManager& operator=(const UserManager&) = delete;


	// ����û�
	bool addUser(User& user);

	// �����ݿ� ���������û���Ϣ��
	bool loadUsers();


	// �����û���ϵ
	bool loadRelation(int32_t userId, std::set<int32_t>& friends);

	// �ӻ����ȡ�����û���Ϣ
	bool getUserInfoFromCached(const std::string& username, User& user);
	bool getUserInfoFromCached(const int userid, User& user);

	// �� ���ݿ� ��ȡ �����û���Ϣ
	bool getUserInfoFromDB(const std::string& username, User& user);
	bool getUserInfoFromDB(const int userid, User& user);

private:
	std::list<User> _cachedUsers;  // �û���Ϣ����
	int32_t _currentUserId = 10000; //��ǰ���� �û�id
	std::mutex _mutex;     // ���滥����
};

