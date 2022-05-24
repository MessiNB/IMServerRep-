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
	bool readData(T& data);

	template<>
	bool readData(std::string& data);

private:
	string _msg;					// ���������ư�
	int _index = 0;				// ��ǰ������ �ֽ���
};
