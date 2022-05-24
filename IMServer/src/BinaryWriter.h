#pragma once

#include <string>

// ������ ����ɶ����Ƶİ�
class BinaryWriter
{
public:
	BinaryWriter() = default;
	~BinaryWriter() = default;

	template<class T>
	bool writeData(T& data);

	template <>
	bool	writeData(std::string& data);

	size_t size() { return _index };
	std::string getMsg() { return _msg };  
	void clear() { _index = 0, _msg.clear(); }
protected:
	void compress(size_t len, string& out);				// ��С���ݽ���ѹ��

private:
	std::string _msg;				// ��װ�İ�
	uint32_t _index = 0;
};
