#include "QueryResult.h"

QueryResult::QueryResult(MYSQL_RES* res = NULL, MyLong rowCount = 0, uint32_t columnCount = 0) : _res(res), _rowCount(rowCount), _columnCount(columnCount)
{
	_currentRow.resize(_columnCount);
	_fieldName.reserve(_columnCount);

	//获取 每个列的信息
	MYSQL_FIELD* fields = mysql_fetch_fields(_res);
	for (size_t i = 0; i < columnCount; i++)
	{
		_fieldName[i] = fields[i].name;  // 获取列名

		//设置type
		_currentRow[i].setType(toIMType(fields[i].type));
		// 设置名称
		_currentRow[i].setName(fields[i].name);
	}
}


QueryResult :: ~QueryResult()
{
	end();
}

bool QueryResult::nextRow()
{
	if (NULL == _res)
		return false;

	MYSQL_ROW row = mysql_fetch_row(_res);  // 获取当前行数据
	if (NULL == row)
	{
		end();  //清除 MYSQL_RES
		return false;
	}

	// 获取每个列中 数据的长度
	//unsigned long* pFieldLength = mysql_fetch_lengths(_res);
	for (size_t i = 0; i < _columnCount; i++)
	{
		if (NULL == row[i])
		{
			// 设置 field 为空
			_currentRow[i].setValue(NULL);
		}
		else
		{
			_currentRow[i].setValue(row[i]);
			//设置 field的值
		}
	}
	return true;
}

Field::IMDataTypes QueryResult::toIMType(enum_field_types mysqlType) const
{

	switch (mysqlType)
	{
	case FIELD_TYPE_TIMESTAMP:
	case FIELD_TYPE_DATE:
	case FIELD_TYPE_TIME:
	case FIELD_TYPE_DATETIME:
	case FIELD_TYPE_YEAR:
	case FIELD_TYPE_STRING:
	case FIELD_TYPE_BLOB:
	case FIELD_TYPE_SET:
		return Field::TYPE_STRING;
	case FIELD_TYPE_NULL:
		return Field::TYPE_NULL;
	case	FIELD_TYPE_TINY: //占一个字节的整数
	case FIELD_TYPE_SHORT:
	case FIELD_TYPE_INT24:
	case FIELD_TYPE_LONG:
	case FIELD_TYPE_LONGLONG:
	case FIELD_TYPE_ENUM:
		return Field::TYPE_INT;
	case FIELD_TYPE_DECIMAL: //十进制小数
	case FIELD_TYPE_FLOAT:
	case FIELD_TYPE_DOUBLE:
		return Field::TYPE_FLOAT;
	default:
		return Field::TYPE_NONE;
	}

	return Field::TYPE_NONE;
}
