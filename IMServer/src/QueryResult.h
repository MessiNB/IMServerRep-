#include <mysql/mysql.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include "muduo/base/Logging.h"
#include "Field.h"

typedef unsigned long long MyLong;

// ��ѯ���ؽ�� �Ĵ���
class QueryResult
{
public:
	
	// ��ѯ������������ ��ʼ������
	QueryResult(MYSQL_RES* res, MyLong rowCount, uint32_t columnCount);
	~QueryResult();

	bool nextRow(); // ��ȡһ������

	// ���� field������׵�ַ(��ǰ�е��׵�ַ��
	Field* Fetch() {
		return _currentRow.data();
	}

	// ʹ���±����
	const Field& operator[] (int index) const
	{
		return _currentRow[index];
	}

	// ʹ�� �ֶ�������
	const Field& operator [] (const std::string& nane) const
	{
		for (size_t i = 0; i < _fieldName.size(); i++)
		{
			if (_fieldName[i] == nane)
				return (*this)[i];
		}
	}

	uint32_t getFieldCount() const { return _columnCount; }

	MyLong getRowCount() const { return _rowCount; }

	std::vector<std::string> getFieldsName()  const { return _fieldName; }

	Field::IMDataTypes toIMType(enum_field_types mysqlType) const;
	
	// �����Խ������ʹ�ã���������
	void end() {
		_rowCount = 0;
		_columnCount = 0;
		_fieldName.clear();
		_currentRow.clear();
		if (_res)
		{
			mysql_free_result(_res);
			_res = NULL;
		}
	}

private:
	std::vector<Field> _currentRow;			// ��ǰ�е�����
	std::vector <std::string> _fieldName; // ��������
	MYSQL_RES* _res = NULL; // �����
	MyLong _rowCount = 0; // ����
	uint32_t _columnCount = 0; // ����
};

typedef std::shared_ptr <QueryResult> QueryResultPtr;
