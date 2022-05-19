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
	if (_index + sizeof(T) > _msg.size())  // ��Ϣ����û������
		return false;
	memcpy(&data, _msg.c_str() + _index, sizeof(T));
	_index += sizeof(T);
}


template<>
bool BinaryReader::readData(std::string& data)
{
	memcpy((char*)data, _msg.c_str() + _index, data.size());
	_index += data.size();
	// TODO������ṹ������
	return true;
}