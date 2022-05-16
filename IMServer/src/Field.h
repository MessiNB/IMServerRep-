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
		TYPE_NONE = 0,  //不知道的类型
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
	IMDataTypes getType()  const { return _type; }					// 返回字段类型
	const std::string& getName() const { return _name; }		// 返回字段名 
	const std::string& getValue() const { return _value; }			// 返回字段值
	
	bool toBool() const { return atoi(_value.c_str()) != 0;  }		// 返回 bool 值，如果值为0 返回 false
	int8_t toInt8() const { return static_cast<int8_t> (atoi(_value.c_str())); }
	int32_t toInt32() const { return atoi(_value.c_str()); }
	int64_t toInt64() const { return atoll(_value.c_str()); }

	bool isEmpty() const { return _isEmpty; }						// 字段值是否为空

private:
	std::string _name;  // 字段名
	std::string _value;	  // 值
	IMDataTypes _type = TYPE_NONE;	  // 类型（默认未知类型）
	bool  _isEmpty = true; // 值是否为空
};
