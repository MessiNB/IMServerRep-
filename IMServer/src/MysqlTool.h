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

// 数据库 工具类（封装下层接口）
class MysqlTool
{
public:
	MysqlTool();
	~MysqlTool();

	
	bool connect(const std::string& host, const std::string& user, const std::string& pwd, const std::string& dbName,uint32_t = 3306);

	
	QueryResultPtr query(const std::string& sql);

	// ִ�����
	bool execute(const std::string& sql);
	bool execute(const std::string& sql, MyLong& nAffectedRowCount, uint32_t& nErrno);

	// 断开连接
	void closeDB()
	{
		if (_mysql != NULL)
		{
			mysql_close(_mysql);
			_mysql = NULL;
		}
	}

	std::string getDBName() const { return _dbname; }

private:
	MYSQL* _mysql = NULL;  // 
	std::string _host;
	std::string _user;
	std::string _pwd;
	std::string _dbname;
	std::string _port;
};
