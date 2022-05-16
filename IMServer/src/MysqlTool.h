#include <mysql/mysql.h>
#include <mysql/errmsg.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <iostream>
#include <string>
#include "muduo/base/Logging.h"
#include "QueryResult.h"

// 封装数据库API  供业务层使用
class MysqlTool
{
public:
	MysqlTool();
	~MysqlTool();

	// 连接事件
	bool connect(const std::string& host, const std::string& user, const std::string& pwd, const std::string& dbName,const uint32_t = 3306);

	// 查询返回 值返回指针，避险查询结果过大的开销
	QueryResultPtr query(const std::string& sql);

	// 执行语句
	bool execute(const std::string& sql);
	bool execute(const std::string& sql, MyLong& nAffectedRowCount, uint32_t& nErrno);

	// 关闭连接
	void closeDB()
	{
		if (_mysql != NULL)
		{
			mysql_close(_mysql);
			_mysql = NULL;
		}
	}

	string getDBName(const std::string& dbName) const { return _dbname; }

private:
	MYSQL* _mysql = NULL;  // 数据库句柄
	std::string _host;
	std::string _user;
	std::string _pwd;
	std::string _dbname;
	std::string _port;
};
