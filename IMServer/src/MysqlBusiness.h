#include "MysqlTool.h"
#include <map>
using std::string;

// 数据库业务层（上层对外接口）
class MysqlManager
{
public:
	struct sFieldInfo
	{
		string sName; // 字段名
		string sType;	// 字段类型
		string sDesc;	// 字段注释描述
	};

	struct sTableInfo
	{
		string sName; // 表名
		string sKey; // 表中主键
		std::map<string, sFieldInfo> mFields;  // 字段名映射表
	};

	MysqlManager();
	~MysqlManager()=default;

	// 初始化链接数据库
	bool init(const string& host, const string& user, const string& pwd, const string& dbname,
		const unsigned int	 port = 3306);

	QueryResultPtr Query(const string& sql);

private:
	bool checkDatabase();					// 检查数据库是否存在
	bool checkTable(const std::string tableName);		// 检查表是否存在
	bool createDataBase();										//  创建数据库
	bool createTable(const sTableInfo& info);			// 创建表
	bool updateTable(const sTableInfo& info);		// 更新表

private:
	std::map <string, sTableInfo> _mTables;   // 通过表名映射
	std::shared_ptr<MysqlTool> _mysqlToolPtr; // sql 工具类共享指针
};

typedef std::pair<string, MysqlManager::sFieldInfo> PairTAble;