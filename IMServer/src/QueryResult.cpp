#include "QueryResult.h"

QueryResult::QueryResult(MYSQL_RES* res = NULL, MyLong rowCount = 0, uint32_t columnCount = 0) : _res(res), _rowCount(rowCount), _columnCount(columnCount)
{
	_currentRow.resize(_columnCount);
	_fieldName.reserve(_columnCount);

	//��ȡ ÿ���е���Ϣ
	MYSQL_FIELD* fields = mysql_fetch_fields(_res);
	for (size_t i = 0; i < columnCount; i++)
	{
		_fieldName[i] = fields[i].name;  // ��ȡ����

		//����type
		_currentRow[i].setType(toIMType(fields[i].type));
		// ��������
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

	MYSQL_ROW row = mysql_fetch_row(_res);  // ��ȡ��ǰ������
	if (NULL == row)
	{
		end();  //��� MYSQL_RES
		return false;
	}

	// ��ȡÿ������ ���ݵĳ���
	//unsigned long* pFieldLength = mysql_fetch_lengths(_res);
	for (size_t i = 0; i < _columnCount; i++)
	{
		if (NULL == row[i])
		{
			// ���� field Ϊ��
			_currentRow[i].setValue(NULL);
		}
		else
		{
			_currentRow[i].setValue(row[i]);
			//���� field��ֵ
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
	case	FIELD_TYPE_TINY: //ռһ���ֽڵ�����
	case FIELD_TYPE_SHORT:
	case FIELD_TYPE_INT24:
	case FIELD_TYPE_LONG:
	case FIELD_TYPE_LONGLONG:
	case FIELD_TYPE_ENUM:
		return Field::TYPE_INT;
	case FIELD_TYPE_DECIMAL: //ʮ����С��
	case FIELD_TYPE_FLOAT:
	case FIELD_TYPE_DOUBLE:
		return Field::TYPE_FLOAT;
	default:
		return Field::TYPE_NONE;
	}

	return Field::TYPE_NONE;
}
