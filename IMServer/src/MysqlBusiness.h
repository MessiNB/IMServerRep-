#include "MysqlTool.h"
#include <map>
using std::string;

// ���ݿ�ҵ��㣨�ϲ����ӿڣ�
class MysqlManager
{
public:
	struct sFieldInfo
	{
		string sName; // �ֶ���
		string sType;	// �ֶ�����
		string sDesc;	// �ֶ�ע������
	};

	struct sTableInfo
	{
		string sName; // ����
		string sKey; // ��������
		std::map<string, sFieldInfo> mFields;  // �ֶ���ӳ���
	};

	MysqlManager();
	~MysqlManager()=default;

	// ��ʼ���������ݿ�
	bool init(const string& host, const string& user, const string& pwd, const string& dbname,
		const unsigned int	 port = 3306);

	QueryResultPtr Query(const string& sql);

private:
	bool checkDatabase();					// ������ݿ��Ƿ����
	bool checkTable(const std::string tableName);		// �����Ƿ����
	bool createDataBase();										//  �������ݿ�
	bool createTable(const sTableInfo& info);			// ������
	bool updateTable(const sTableInfo& info);		// ���±�

private:
	std::map <string, sTableInfo> _mTables;   // ͨ������ӳ��
	std::shared_ptr<MysqlTool> _mysqlToolPtr; // sql �����๲��ָ��
};

typedef std::pair<string, MysqlManager::sFieldInfo> PairTAble;