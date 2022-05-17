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

// 查询返回结果 的处理
class QueryResult
{
public:
	
	// 查询的行数，列数 初始化对象
	QueryResult(MYSQL_RES* res, MyLong rowCount, uint32_t columnCount);
	~QueryResult();

	bool nextRow(); // 获取一行数据

	// 返回 field数组的首地址(当前行的首地址）
	Field* Fetch() {
		return _currentRow.data();
	}

	// 使用下标访问
	const Field& operator[] (int index) const
	{
		return _currentRow[index];
	}

	// 使用 字段名访问
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
	
	// 结束对结果集的使用，清理结果集
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
	std::vector<Field> _currentRow;			// 当前行的数据
	std::vector <std::string> _fieldName; // 列名数组
	MYSQL_RES* _res = NULL; // 结果集
	MyLong _rowCount = 0; // 行数
	uint32_t _columnCount = 0; // 列数
};

typedef std::shared_ptr <QueryResult> QueryResultPtr;
