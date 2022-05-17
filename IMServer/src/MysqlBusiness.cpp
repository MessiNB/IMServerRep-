#include "MysqlBusiness.h"
#include <sstream>
MysqlManager::MysqlManager()
{
	sTableInfo info;

	//用户信息
	info.sName = "users";
	info.mFields["id"] = { "id", "bigint(20) NOTNULL AUTO_INCREMENT COMMENT '自增id' ", "bigint(20)" };
	info.mFields["user_id"] = { "user_id", "bigint(20) NOTNULL   COMMENT '用户id' ", "bigint(20)" };
	info.mFields["user_name"] = { "user_nickname", "varchar(64) NOTNULL   COMMENT '用户名' ", "varchar(64)" };
	info.mFields["user_nickname"] = { "user_name", "varchar(64) NOTNULL   COMMENT '用户昵称' ", "varchar(64)" };
	info.mFields["user_password"] = { "user_password", "varchar(64) NOTNULL   COMMENT '用户密码' ", "varchar(64)" };
	info.mFields["user_facetype"] = { "user_facetype", "int(10) DEFAULT 0   COMMENT '头像类型' ", "int(10)" };
	info.mFields["user_customface"] = { "user_customface", "varchar(32) DEFAULT NULL   COMMENT '自定义头像名' ", "varchar(32)" };
	info.mFields["user_gender"] = { "user_gender", "int(2) DEFAULT 0   COMMENT '性别 ", "int(2)" };
	info.mFields["user_birthday"] = { "user_birthday", "int(11) DEFAULT 20000101   COMMENT '生日' ", "int(11)" };
	info.mFields["user_address"] = { "user_address", "varchar(256) DEFAULT NULL  COMMENT '地址' ", "varcahr(256)" };
	info.mFields["user_phone"] = { "user_phone", "varchar(11) D EFAULT NULL  COMMENT '手机' ", "varcahr(11)" };
	info.mFields["user_email"] = { "user_email", "varchar(256) DEFAULT NULL  COMMENT '邮箱' ", "varcahr(256)" };
	info.mFields["user_email"] = { "user_email", "datetime NOTNULL  COMMENT '注册时间' ", "datetime" };
	info.sKey = "PRIMARY KEY (user_id), INDEX user_id (user_id), KEY id (id)";
	_mTables.insert(PairTAble(info.sName, info));

	//聊天内容
	info.mapField.clear();
	info.sName = "chatmsg";
	info.mapField["id"] = { "id", "bigint(20) NOT NULL AUTO_INCREMENT COMMENT '自增ID'", "bigint(20)" };
	info.mapField["senderid"] = { "senderid", "bigint(20) NOT NULL COMMENT '发送者id'", "bigint(20)" };
	info.mapField["targetid"] = { "targetid", "bigint(20) NOT NULL COMMENT '接收者id'", "bigint(20)" };
	info.mapField["msgcontent"] = { "msgcontent", "blob NOT NULL COMMENT '聊天内容'", "blob" };
	info.mapField["create_time"] = { "create_time", "timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '更新时间'", "timestamp" };
	info.mapField["remark"] = { "remark", "varchar(64) NULL COMMENT '备注'", "varchar(64)" };
	info.sKey = "PRIMARY KEY (id), INDEX id (id)";
	m_mapTable.insert(TablePair(info.sName, info));

	//用户关系
	info.mapField.clear();
	info.sName = "user_relationship";
	info.mapField["id"] = { "id", "bigint(20) NOT NULL AUTO_INCREMENT COMMENT '自增ID'", "bigint(20)" };
	info.mapField["user_id1"] = { "user_id1", "bigint(20) NOT NULL COMMENT '用户ID'", "bigint(20)" };
	info.mapField["user_id2"] = { "user_id2", "bigint(20) NOT NULL COMMENT '用户ID'", "bigint(20)" };
	info.mapField["remark"] = { "remark", "varchar(64) NULL COMMENT '备注'", "varchar(64)" };
	info.mapField["create_time"] = { "create_time", "timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '更新时间'", "timestamp" };
	info.sKey = "PRIMARY KEY (id), INDEX id (id)";
	m_mapTable.insert(TablePair(info.sName, info));
}

bool MysqlManager::init(const string& host, const string& user, const string& pwd, const string& dbname, const unsigned int port = 3306)
{
	_mysqlToolPtr.reset(new MysqlTool);
	bool res = _mysqlToolPtr->connect(host, user, pwd, dbname, port);
	if (!res)
		return false;

	// TODO: 数据库是否存在
	if (checkDatabase() == false)
	{
		if (createDataBase() == false)
			return false;
	}
	// TODO: 表是否存在
	std::map < std::string, MysqlManager::sTableInfo>::iterator it = _mTables.begin();
	for (; it < _mTables.end(); it++)
	{
		if (!checkTable(it->second.sName))
		{
			if (!createTable(it->second))
				return false;
		}
	}
	return true;
}

bool MysqlManager::checkDatabase()
{
	if (NULL == _mysqlToolPtr)
		return false;
	QueryResultPtr reslut = _mysqlToolPtr->query("show databases;");
	if (NULL = reslut)
		return false;

	Field* row = reslut->Fetch();
	std::string	 dbName = _mysqlToolPtr->getDBName();

	if (NULL != row)
	{
		if (reslut->nextRow() == false)
			break;
		if (dbName == row[0].getValue())
		{
			reslut->end();
			return true;
		}
	}
	reslut->end();
	std::cout << "dataBase not found" << std::endl;
	return false;
}

bool MysqlManager::checkTable(const std::string tableName)
{
	if (_mysqlToolPtr == NULL) return false;
	std::stringstream sql;
	sql << "desc " << tableName << ";";
	QueryResultPtr result = _mysqlToolPtr->query(sql);

	//如果表不存在
	if (NULL == result)
	{
		return false;
	}

	std::map<std::string, sFieldInfo> map_reset(_mTables[tableName]);  // 要追加的
	std::map<std::string, sFieldInfo> map_change;									  // 要更新的

	// 检查表是否正确, 字段名，字段类型
	Field* fields = result->Fetch();
	while (NULL != fields)
	{
		if (result->nextRow() == false)
			break;
		std::string name = fields[0].getValue();  //数据库中字段名
		std::string type = fields[1].getValue();  //数据库中字段类型
		auto it = _mTables[tableName].mFields.find(name);
		if (it == _mTables[tableName].mFields.end())
		{
			// TODO: 代中没有该字段
			continue;
		}
		map_reset.erase(name);  // 数据库中和代码中都有，从 要追加的中删除
		if (it->second.sType != type)
		{
			// 字段类型不同，修改字段类型
			map_change.insert(PairTAble(name, it->second));
		}
	}
	result->end();

	if (map_reset.size() > 0)
	{
		auto it = map_reset.begin();
		std::stringstream reset_sql;
		for (; it < map_reset.end(); it++)
		{
			reset_sql << "alter table " << tableName << " add column " << it->second.sDesc << " " << it->second.sType << ";";
			if (!_mysqlToolPtr->execute(reset_sql))
				return false;
		}
	}

	if (map_change.size() > 0)
	{
		auto it = map_change.begin();
		std::stringstream change_sql;
		change_sql << "alter table " << tableName << " modify column " << it->second.sDesc << " " << it->second.sType << ";";
		if (!_mysqlToolPtr->execute(change_sql))
			return false;
	}
	return true;
}

bool MysqlManager::createDataBase()
{
	if (_mysqlToolPtr == NULL)
		return false;
	std::stringstream sql;
	sql << "create database " << _mysqlToolPtr->getDBName() << ";";
	MyLong affectRow = 0;
	uint32_t nErrno = 0;
	if (_mysqlToolPtr->execute(sql, affectRow, nErrno));
	if (affectRow == 1)	return  true;
	return false;
}

bool MysqlManager::createTable(const sTableInfo& info)
{
	if (NULL == _mysqlToolPtr)		return false;
	if (0 == info.mFields.size()) return false;

	std::stringstream sql;
	sql << "create table if not exists " << info.sName << "(";
	const std::map<std::string, sFieldInfo >::iterator it = info.mFields.begin();

	for ( ; it < info.mFields.end() ; it++ )
	{
		if (it != info.mFields.begin())
			sql << ",";
		sFieldInfo field = it->second;
		sql << field.sName << " " << field.sType;
	}

	if (info.sKey.size() > 0)
	{
		sql << "," << info.sKey;
	}
	sql << ") default charset=utf-8,ENGINE=InnoDB;";
	return _mysqlToolPtr->execute(sql);
}

