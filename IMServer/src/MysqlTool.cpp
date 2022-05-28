#include "MysqlTool.h"

MysqlTool::MysqlTool()
{

}

MysqlTool :: ~MysqlTool()
{
	closeDB();
}

bool MysqlTool::connect(const std::string& host, const std::string& user, 
const std::string& pwd, const std::string& dbName, const uint32_t port)
{
	closeDB();
	// 初始化 句柄
	_mysql = mysql_init(_mysql);

	//����
	_mysql = mysql_real_connect(_mysql, host.c_str(), user.c_str(), pwd.c_str(), dbName.c_str(), port, NULL, 0);
<<<<<<< HEAD
	cout << "mysql connect error：" << mysql_error(_mysql);
=======
	int sql_errno = mysql_errno(_mysql);
        std::cout << "mysql connect error :[" << sql_errno <<":]" << mysql_error(_mysql) << std::endl;
>>>>>>> 53976e84ed0b41cf4ce4ac00f7bab6c23eea0e5c
	std::cout << "[host: " << host << "], [user: " << user << "] , [pwd: " << pwd << "] , [dbname: " << dbName << "]" << std::endl;

	if (_mysql != NULL)
	{ 
		//���ӳɹ������ô�ʱ�� ��������
		_host = host;
		_user = user;
		_pwd = pwd;
		_dbname = dbName;
		_port = port;
		
		mysql_query(_mysql, "set names utf8");  //  ���ñ��뼯��֧������)
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
			return QueryResultPtr();  // 失败返回空的 共享指针
	}
	int ret = mysql_real_query(_mysql, sql.c_str(), sql.size()); //二进制 查询
	if (ret != 0) {
		// 非0表示 失败
		uint32_t nErrno = mysql_errno(_mysql); //获取错误 code
		std::cout << "[" << sql << "]  failed , errno = " << nErrno << " , error = " << mysql_error(_mysql) << std::endl;

		// 如果是服务器的原因（比如网络断开），再重连一次
		if (CR_SERVER_GONE_ERROR == nErrno)
		{
			if (connect(_host, _user,_pwd ,_dbname) == false)
				return QueryResultPtr();  

			ret = mysql_real_query(_mysql, sql.c_str(), sql.size());
			if (0 != ret)
			{
				nErrno = mysql_errno(_mysql); 
				std::cout << "[" << sql << "]  failed , errno = " << nErrno << " , error = " << mysql_error(_mysql) << std::endl;
				return QueryResultPtr();
			}
		}
		else
		{
			// 不是服务器的原因，直接返回 空指针
			return QueryResultPtr();
		}
	}
	MYSQL_RES* res = mysql_store_result(_mysql);				// 获取结果集
	MyLong  rowCount = mysql_affected_rows(_mysql);		// 获取行数
	if (rowCount == 0)
		return QueryResultPtr();
	uint32_t columnCount = mysql_field_count(_mysql);		// 获取列数

	return QueryResultPtr(new QueryResult(res, rowCount, columnCount));
}

// ִ�����
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
		if (connect(_host, _user, _pwd,_dbname) == false)
			return false;  // ���Ӳ������ݿ� ���ؿյ� shard_ptr
	}

	int ret = mysql_query(_mysql, sql.c_str()); //��ѯ�ɹ�����0
	if (ret != 0) {
		// ��ѯʧ��
		  nErrno = mysql_errno(_mysql); //��ȡ���� code
		std::cout << "[" << sql << "]  failed , errno = " << nErrno << " , error = " << mysql_error(_mysql) << std::endl;

		// ��� �� ��������ԭ�򣬱�������Ͽ�����������
		if (CR_SERVER_GONE_ERROR == nErrno)
		{
			if (connect(_host, _user, _pwd,_dbname) == false)
				return false;  // ���Ӳ������ݿ� ���ؿյ� shard_ptr

			ret = mysql_query(_mysql, sql.c_str());
			if (0 != ret)
			{
				nErrno = mysql_errno(_mysql); //��ȡ���� code
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

