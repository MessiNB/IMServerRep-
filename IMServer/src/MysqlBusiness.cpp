#include "MysqlBusiness.h"
#include <sstream>
MysqlManager::MysqlManager()
{
	sTableInfo info;
	//用户
	info.sName = "user";
	info.mFields["id"] = { "id","bigint(20) NOT NULL AUTO_INCREMENT COMMENT '自增ID'","bigint(20)" };
	info.mFields["user_id"] = { "user_id", "bigint(20) NOT NULL COMMENT '用户ID'", "bigint(20)" };
	info.mFields["username"] = { "username", "varchar(64) NOT NULL COMMENT '用户名'", "varchar(64)" };
	info.mFields["nickname"] = { "nickname", "varchar(64) NOT NULL COMMENT '用户昵称'", "varchar(64)" };
	info.mFields["password"] = { "password", "varchar(64) NOT NULL COMMENT '用户密码'", "varchar(64)" };
	info.mFields["facetype"] = { "facetype", "int(10) DEFAULT 0 COMMENT '用户头像类型'", "int(10)" };
	info.mFields["customface"] = { "customface", "varchar(32) DEFAULT NULL COMMENT '自定义头像名'", "varchar(32)" };
	info.mFields["customfacefmt"] = { "customfacefmt", "varchar(6) DEFAULT NULL COMMENT '自定义头像格式'", "varchar(6)" };
	info.mFields["gender"] = { "gender", "int(2)  DEFAULT 0 COMMENT '性别'", "int(2)" };
	info.mFields["birthday"] = { "birthday", "bigint(20)  DEFAULT 19900101 COMMENT '生日'", "bigint(20)" };
	info.mFields["signature"] = { "signature", "varchar(256) DEFAULT NULL COMMENT '地址'", "varchar(256)" };
	info.mFields["address"] = { "address", "varchar(256) DEFAULT NULL COMMENT '地址'", "varchar(256)" };
	info.mFields["phonenumber"] = { "phonenumber", "varchar(64) DEFAULT NULL COMMENT '电话'", "varchar(64)" };
	info.mFields["mail"] = { "mail", "varchar(256) DEFAULT NULL COMMENT '邮箱'", "varchar(256)" };
	info.mFields["owner_id"] = { "owner_id", "bigint(20) DEFAULT 0 COMMENT '群账号群主userid'", "bigint(20)" };
	info.mFields["register_time"] = { "register_time", "datetime NOT NULL COMMENT '注册时间'", "datetime" };
	info.mFields["remark"] = { "remark", "varchar(64) NULL COMMENT '备注'", "varchar(64)" };
	info.mFields["update_time"] = { "update_time", "timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '更新时间'", "timestamp" };
	info.sKey = "PRIMARY KEY (user_id), INDEX user_id (user_id), KEY  id  ( id )";
	_mTables.insert(PairTAble(info.sName, info));
	//聊天内容
	info.mFields.clear();
	info.sName = "chatmsg";
	info.mFields["id"] = { "id", "bigint(20) NOT NULL AUTO_INCREMENT COMMENT '自增ID'", "bigint(20)" };
	info.mFields["senderid"] = { "senderid", "bigint(20) NOT NULL COMMENT '发送者id'", "bigint(20)" };
	info.mFields["targetid"] = { "targetid", "bigint(20) NOT NULL COMMENT '接收者id'", "bigint(20)" };
	info.mFields["msgcontent"] = { "msgcontent", "blob NOT NULL COMMENT '聊天内容'", "blob" };
	info.mFields["create_time"] = { "create_time", "timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '更新时间'", "timestamp" };
	info.mFields["remark"] = { "remark", "varchar(64) NULL COMMENT '备注'", "varchar(64)" };
	info.sKey = "PRIMARY KEY (id), INDEX id (id)";
	_mTables.insert(PairTAble(info.sName, info));

	//用户关系
	info.mFields.clear();
	info.sName = "user_relationship";
	info.mFields["id"] = { "id", "bigint(20) NOT NULL AUTO_INCREMENT COMMENT '自增ID'", "bigint(20)" };
	info.mFields["user_id1"] = { "user_id1", "bigint(20) NOT NULL COMMENT '用户ID'", "bigint(20)" };
	info.mFields["user_id2"] = { "user_id2", "bigint(20) NOT NULL COMMENT '用户ID'", "bigint(20)" };
	info.mFields["remark"] = { "remark", "varchar(64) NULL COMMENT '备注'", "varchar(64)" };
	info.mFields["create_time"] = { "create_time", "timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '更新时间'", "timestamp" };
	info.sKey = "PRIMARY KEY (id), INDEX id (id)";
	_mTables.insert(PairTAble(info.sName, info));
}

bool MysqlManager::init(const string& host, const string& user, const string& pwd, const string& dbname, const uint32_t port)
{
	_mysqlToolPtr.reset(new MysqlTool);
	bool res = _mysqlToolPtr->connect(host, user, pwd, dbname, port);
	if (!res)
		return false;

	// TODO: 锟斤拷锟捷匡拷锟角凤拷锟斤拷锟?
	if (checkDatabase() == false)
	{
		if (createDataBase() == false)
			return false;
	}
	// TODO: 锟斤拷锟角凤拷锟斤拷锟?
	std::map < std::string, MysqlManager::sTableInfo>::iterator it = _mTables.begin();
	for (; it != _mTables.end(); it++)
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
	if (NULL == reslut)
		return false;

	Field* row = reslut->Fetch();
	std::string	 dbName = _mysqlToolPtr->getDBName();

	while (NULL != row)
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
	QueryResultPtr result = _mysqlToolPtr->query(sql.str());

	//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟?
	if (NULL == result)
	{
		return false;
	}

	std::map<std::string, sFieldInfo> map_reset(_mTables[tableName].mFields);  // 要追锟接碉拷
	std::map<std::string, sFieldInfo> map_change;									  // 要锟斤拷锟铰碉拷

	// 锟斤拷锟斤拷锟角凤拷锟斤拷确, 锟街讹拷锟斤拷锟斤拷锟街讹拷锟斤拷锟斤拷
	Field* fields = result->Fetch();
	while (NULL != fields)
	{
		if (result->nextRow() == false)
			break;
		std::string name = fields[0].getValue();  //锟斤拷锟捷匡拷锟斤拷锟街讹拷锟斤拷
		std::string type = fields[1].getValue();  //锟斤拷锟捷匡拷锟斤拷锟街讹拷锟斤拷锟斤拷
		auto it = _mTables[tableName].mFields.find(name);
		if (it == _mTables[tableName].mFields.end())
		{
			// TODO: 锟斤拷锟斤拷没锟叫革拷锟街讹拷
			continue;
		}
		map_reset.erase(name);  // 锟斤拷锟捷匡拷锟叫和达拷锟斤拷锟叫讹拷锟叫ｏ拷锟斤拷 要追锟接碉拷锟斤拷删锟斤拷
		if (it->second.sType != type)
		{
			// 锟街讹拷锟斤拷锟酵诧拷同锟斤拷锟睫革拷锟街讹拷锟斤拷锟斤拷
			map_change.insert(PairFiled(name, it->second));
		}
	}
	result->end();

	if (map_reset.size() > 0)
	{
		auto it = map_reset.begin();
		std::stringstream reset_sql;
		for (; it!=map_reset.end(); it++)
		{
			reset_sql << "alter table " << tableName << " add column " << it->second.sDesc << " " << it->second.sType << ";";
			if (!_mysqlToolPtr->execute(reset_sql.str()))
				return false;
		}
	}

	if (map_change.size() > 0)
	{
		auto it = map_change.begin();
		std::stringstream change_sql;
		change_sql << "alter table " << tableName << " modify column " << it->second.sDesc << " " << it->second.sType << ";";
		if (!_mysqlToolPtr->execute(change_sql.str()))
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
	if (_mysqlToolPtr->execute(sql.str(), affectRow, nErrno));
	if (affectRow == 1)	return  true;
	return false;
}

bool MysqlManager::createTable(const sTableInfo& info)
{
	if (NULL == _mysqlToolPtr)		return false;
	if (0 == info.mFields.size()) return false;

	std::stringstream sql;
	sql << "create table if not exists " << info.sName << "(";
	 std::map<std::string, sFieldInfo >::const_iterator it = info.mFields.begin();

	for ( ; it != info.mFields.end() ; it++)
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
	return _mysqlToolPtr->execute(sql.str());
}

