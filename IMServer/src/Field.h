#include <mysql/mysql.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <string>
#include <memory>
#include <iostream>
#include "muduo/base/Logging.h"


class Field
{
public:
	typedef enum
	{
		TYPE_NONE = 0,  //��֪��������
		TYPE_STRING,
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_BOOL,
		TYPE_NULL
	}IMDataTypes ;

	Field()=default;
	~Field()=default;
	void setType ( IMDataTypes tp) { _type = tp; }
	void setName (const std::string name)  {  _name = name; }
	void setValue(const char* value);
	IMDataTypes getType()  const { return _type; }					// �����ֶ�����
	const std::string& getName() const { return _name; }		// �����ֶ��� 
	const std::string& getValue() const { return _value; }			// �����ֶ�ֵ
	
	bool toBool() const { return atoi(_value.c_str()) != 0;  }		// ���� bool ֵ�����ֵΪ0 ���� false
	int8_t toInt8() const { return static_cast<int8_t> (atoi(_value.c_str())); }
	int32_t toInt32() const { return atoi(_value.c_str()); }
	int64_t toInt64() const { return atoll(_value.c_str()); }

	bool isEmpty() const { return _isEmpty; }						// �ֶ�ֵ�Ƿ�Ϊ��

private:
	std::string _name;  // �ֶ���
	std::string _value;	  // ֵ
	IMDataTypes _type = TYPE_NONE;	  // ���ͣ�Ĭ��δ֪���ͣ�
	bool  _isEmpty = true; // ֵ�Ƿ�Ϊ��
};
