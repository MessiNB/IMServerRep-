#include "MysqlTool.h"

MysqlTool::MysqlTool()
{

}

MysqlTool :: ~MysqlTool()
{
	closeDB();
}

bool MysqlTool::connect(const std::string& host, const std::string& user, const std::string& pwd, const std::string& dbName,const uint32_t port =3306)
{
	closeDB();
	// 初始化
	_mysql = mysql_init(_mysql);

	//连接
	_mysql = mysql_real_connect(_mysql, host.c_str(), user.c_str(), pwd.c_str(), dbName.c_str(), port, NULL, 0);

	std::cout << "[host: " << host << "], [user: " << user << "] , [pwd: " << pwd << "] , [dbname: " << dbName << "]" << std::endl;

	if (_mysql != NULL)
	{ 
		//链接成功，设置此时的 链接数据
		_host = host;
		_user = user;
		_pwd = pwd;
		_dbname = dbName;
		_port = port;
		
		mysql_query(_mysql, "set names utf8");  //  设置编码集（支持中文)
		std::cout << "mysql connect success !" << std::endl;
		return true;
	}
	std::cout << "mysql connect failed !" << std::endl;
	return false;
}

QueryResultPtr MysqlTool::query(const std::string& sql)
{
	if (NULL == _mysql)
	{
		if (connect(_host, _user,_pwd ,_dbname) == false)
			return QueryResultPtr();  // 连接不上数据库 返回空的 shard_ptr
	}
	int ret = mysql_real_query(_mysql, sql.c_str(), sql.size()); //查询成功返回0
	if (ret != 0) {
		// 查询失败
		uint32_t nErrno = mysql_errno(_mysql); //获取错误 code
		std::cout << "[" << sql << "]  failed , errno = " << nErrno << " , error = " << mysql_error(_mysql) << std::endl;

		// 如果 是 服务器的原因，比如网络断开，尝试重来
		if (CR_SERVER_GONE_ERROR == nErrno)
		{
			if (connect(_host, _user,_pwd ,_dbname) == false)
				return QueryResultPtr();  // 连接不上数据库 返回空的 shard_ptr

			ret = mysql_real_query(_mysql, sql.c_str(), sql.size());
			if (0 != ret)
			{
				nErrno = mysql_errno(_mysql); //获取错误 code
				std::cout << "[" << sql << "]  failed , errno = " << nErrno << " , error = " << mysql_error(_mysql) << std::endl;
				return QueryResultPtr();
			}
		}
		else
		{
			// 不是服务器导致的 查询失败
			return QueryResultPtr();
		}
	}
	MYSQL_RES* res = mysql_store_result(_mysql);				// 获取查询返回结果集
	MyLong  rowCount = mysql_affected_rows(_mysql);		// 获取 行数
	uint32_t columnCount = mysql_field_count(_mysql);		// 获取 列数

	return QueryResultPtr(new QueryResult(res, rowCount, columnCount));
}

// 执行语句
bool MysqlTool::execute(const std::string& sql)
{
	MyLong nAffectedCount;
	unsigned int sqlErrno;
	return execute(sql, nAffectedCount, sqlErrno);
}

bool MysqlTool::execute(const std::string& sql, MyLong& nAffectedRowCount, uint32_t& nErrno)
{
	if (NULL == _mysql)
	{
		if (connect(_host, _user, _pwd,_dbnamse) == false)
			return false;  // 连接不上数据库 返回空的 shard_ptr
	}

	int ret = mysql_query(_mysql, sql.c_str()); //查询成功返回0
	if (ret != 0) {
		// 查询失败
		  nErrno = mysql_errno(_mysql); //获取错误 code
		std::cout << "[" << sql << "]  failed , errno = " << nErrno << " , error = " << mysql_error(_mysql) << std::endl;

		// 如果 是 服务器的原因，比如网络断开，尝试重来
		if (CR_SERVER_GONE_ERROR == nErrno)
		{
			if (connect(_host, _user, _pwd,_dbname) == false)
				return false;  // 连接不上数据库 返回空的 shard_ptr

			ret = mysql_query(_mysql, sql.c_str());
			if (0 != ret)
			{
				nErrno = mysql_errno(_mysql); //获取错误 code
				std::cout << "[" << sql << "]  failed , errno = " << nErrno << " , error = " << mysql_error(_mysql) << std::endl;
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	nAffectedRowCount = mysql_affected_rows(_mysql);
	return true;
}

