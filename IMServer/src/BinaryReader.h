#pragma once

#include <iostream>
#include <string>
using std::string;


// ���������� ��
class BinaryReader
{
public:
	BinaryReader() = default;
	BinaryReader(const string& msg):_msg(msg){}
	~BinaryReader() = default;

	BinaryReader& operator=(const BinaryReader& reader);

	void setMsg(string& msg) { _msg = msg; _index = 0; }
	void reset() { _index = 0; }
	size_t size() { return _msg.size();}


	template<class T>
	bool readData(T& data)
	{
		if (_index + sizeof(T) > _msg.size())  // ��Ϣ����û������
			return false;
		memcpy(&data, _msg.c_str() + _index, sizeof(T));
		_index += sizeof(T);
	}


	template<>
	bool readData(std::string& data)
	{
		memcpy((char*)data.c_str(), _msg.c_str() + _index, data.size());
		_index += data.size();
		// TODO������ṹ������
		return true;
	}

	static void dump(const string& buf) {
		dump(buf.c_str(), buf.size());
	}
	static void dump(const char* buf, size_t size) {
		for (size_t i = 0; i < size; i++) {
			if (i != 0 && ((i % 16) == 0))printf("\r\n");
			printf("%02X ", (unsigned)(buf[i]) & 0xFF);
		}
		printf("\r\n");
	}

private:
	string _msg;					// ���������ư�
	int _index = 0;				// ��ǰ������ �ֽ���
};
