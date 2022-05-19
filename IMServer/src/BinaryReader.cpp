#include "BinaryReader.h"

BinaryReader::operator=(const BinaryReader& reader)
{
		if (this != &reader)
		{
			_msg = reader._msg;
			_msg = reader._index;
		}
		return *this;
}

template<class T>
bool BinaryReader::readData(T& data)
{
	if (_index + sizeof(T) > _msg.size())  // 消息中已没有数据
		return false;
	memcpy(&data, _msg.c_str() + _index, sizeof(T));
	_index += sizeof(T);
}


template<>
bool BinaryReader::readData(std::string& data)
{
	memcpy((char*)data, _msg.c_str() + _index, data.size());
	_index += data.size();
	// TODO：处理结构体数据
	return true;
}