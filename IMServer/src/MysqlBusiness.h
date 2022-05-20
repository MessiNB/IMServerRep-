#include "MysqlTool.h"
#include <map>
using std::string;

// 数据库 业务类（上层接口）
class MysqlManager
{
public:
	struct sFieldInfo
	{
		string sName; // 字段名
		string sType; // 字段类型
		string sDesc; // 字段描述
	};

	struct sTableInfo
	{
		string sName;						  // 表名
		string sKey;						  // 主键和索引
		std::map<string, sFieldInfo> mFields; // 字段映射表
	};
public:
	MysqlManager();
	~MysqlManager() = default;

// 初始化连接
	bool init(const string &host, const string &user, const string &pwd, const string &dbname, const uint32_t port = 3306);

	QueryResultPtr Query(const string &sql)
	{
		if (_mysqlToolPtr == NULL)
			return QueryResultPtr();
		return _mysqlToolPtr->query(sql);
	}

private:
	bool checkDatabase();						  //检查数据库是否存在
	bool checkTable(const std::string tableName); // 检查表
	bool createDataBase();						  //  创建数据库
	bool createTable(const sTableInfo &info);	  // 创建表
												  // bool updateTable(const sTableInfo& info);		// ���±�

private:
	std::map<string, sTableInfo> _mTables;	  //表名 映射表
	std::shared_ptr<MysqlTool> _mysqlToolPtr; // 数据库 工具类共享指针
};

typedef std::pair<string, MysqlManager::sTableInfo> PairTAble;
typedef std::pair<string, MysqlManager::sFieldInfo> PairFiled;
