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
class User
{
public:
	USer() = default;
	~USer() = default;
	int32_t				 _userId = -1;
	std::string			 _userName;
	std::string			 _passWord;
	std::string			 _nickName;
	int32_t				_faceType = -1;  // Ĭ��ͷ��
	std::string			_customFace;
	std::string			 _customFaceFmt; // �Զ���ͷ���ʽ
	int32_t				_gender = 0;  // �Ա� ��Ĭ��Ů�ԣ�
	int32_t				 _birthDay;  // ����
	std::string			 _adress;
	std::string			 _pthone;
	std::string			 _mail;
	int32_t				 _ownerid ;	// ���ڱ��Ⱥ����Ϣ

	set<int32_t>	 _friends;  // ����id

};


class UserManager
{
public:
	UserManager();
	~UserManager();

	bool init();

	UserManager(const UserManager&) = delete;
	UserManager& operator=(const UserManager&) = delete;


	// ����û�
	bool addUser(User& user);

	// �����ݿ� �����û���Ϣ��
	bool loadUsers();


	// �����û���ϵ
	bool loadReles(int32_t userId, const std::set<int32_t>& friends);

private:
	std::list<User> _cachedUsers;  // �û���Ϣ����
};

