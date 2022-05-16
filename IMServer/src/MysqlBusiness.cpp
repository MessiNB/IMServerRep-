#include "MysqlBusiness.h"

MysqlManager::MysqlManager()
{
	sTableInfo info;

	//�û���Ϣ
	info.sName = "users";
	info.mFields["id"] = { "id", "bigint(20) NOTNULL AUTO_INCREMENT COMMENT '����id' ", "bigint(20)" };
	info.mFields["user_id"] = { "user_id", "bigint(20) NOTNULL   COMMENT '�û�id' ", "bigint(20)" };
	info.mFields["user_name"] = { "user_nickname", "varchar(64) NOTNULL   COMMENT '�û���' ", "varchar(64)" };
	info.mFields["user_nickname"] = { "user_name", "varchar(64) NOTNULL   COMMENT '�û��ǳ�' ", "varchar(64)" };
	info.mFields["user_password"] = { "user_password", "varchar(64) NOTNULL   COMMENT '�û�����' ", "varchar(64)" };
	info.mFields["user_facetype"] = { "user_facetype", "int(10) DEFAULT 0   COMMENT 'ͷ������' ", "int(10)" };
	info.mFields["user_customface"] = { "user_customface", "varchar(32) DEFAULT NULL   COMMENT '�Զ���ͷ����' ", "varchar(32)" };
	info.mFields["user_gender"] = { "user_gender", "int(2) DEFAULT 0   COMMENT '�Ա� ", "int(2)" };
	info.mFields["user_birthday"] = { "user_birthday", "int(11) DEFAULT 20000101   COMMENT '����' ", "int(11)" };
	info.mFields["user_address"] = { "user_address", "varchar(256) DEFAULT NULL  COMMENT '��ַ' ", "varcahr(256)" };
	info.mFields["user_phone"] = { "user_phone", "varchar(11) D EFAULT NULL  COMMENT '�ֻ�' ", "varcahr(11)" };
	info.mFields["user_email"] = { "user_email", "varchar(256) DEFAULT NULL  COMMENT '����' ", "varcahr(256)" };
	info.mFields["user_email"] = { "user_email", "datetime NOTNULL  COMMENT 'ע��ʱ��' ", "datetime" };
	info.sKey = "PRIMARY KEY (user_id), INDEX user_id (user_id), KEY id (id)";
	_mTables.insert(PairTAble(info.sName,info));

	//��������
	info.mapField.clear();
	info.sName = "chatmsg";
	info.mapField["id"] = { "id", "bigint(20) NOT NULL AUTO_INCREMENT COMMENT '����ID'", "bigint(20)" };
	info.mapField["senderid"] = { "senderid", "bigint(20) NOT NULL COMMENT '������id'", "bigint(20)" };
	info.mapField["targetid"] = { "targetid", "bigint(20) NOT NULL COMMENT '������id'", "bigint(20)" };
	info.mapField["msgcontent"] = { "msgcontent", "blob NOT NULL COMMENT '��������'", "blob" };
	info.mapField["create_time"] = { "create_time", "timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '����ʱ��'", "timestamp" };
	info.mapField["remark"] = { "remark", "varchar(64) NULL COMMENT '��ע'", "varchar(64)" };
	info.sKey = "PRIMARY KEY (id), INDEX id (id)";
	m_mapTable.insert(TablePair(info.sName, info));

	//�û���ϵ
	info.mapField.clear();
	info.sName = "user_relationship";
	info.mapField["id"] = { "id", "bigint(20) NOT NULL AUTO_INCREMENT COMMENT '����ID'", "bigint(20)" };
	info.mapField["user_id1"] = { "user_id1", "bigint(20) NOT NULL COMMENT '�û�ID'", "bigint(20)" };
	info.mapField["user_id2"] = { "user_id2", "bigint(20) NOT NULL COMMENT '�û�ID'", "bigint(20)" };
	info.mapField["remark"] = { "remark", "varchar(64) NULL COMMENT '��ע'", "varchar(64)" };
	info.mapField["create_time"] = { "create_time", "timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '����ʱ��'", "timestamp" };
	info.sKey = "PRIMARY KEY (id), INDEX id (id)";
	m_mapTable.insert(TablePair(info.sName, info));
}

bool MysqlManager::init(const string& host, const string& user, const string& pwd, const string& dbname, const unsigned int port)
{
	_mysqlToolPtr.reset(new MysqlTool);
	bool res = _mysqlToolPtr->connect(host, user, pwd, dbname, port);
	if (!res)
		return false;

	// TODO: ���ݿ��Ƿ����
	if (checkDatabase() == false)
	{
		if (createDataBase() == false)
			return false;
	}
	// TODO: ���Ƿ����
	std::map < std::string, MysqlManager::sTableInfo>::iterator it = _mTables.begin();
	for ( ; it < _mTables.end(); it++)
	{
		if ( !checkTable(it->second.sName))
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
	QueryResult reslut = _mysqlToolPtr->query("show databases;");
	if (NULL = reslut)
		return false;

	return false;
}

bool MysqlManager::init(const string& host, const string& user, const string& pwd, const string& dbname, short port)
{

	return false;
}
