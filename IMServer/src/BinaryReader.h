#include <iostream>
#include <string>
using std::string;

class BinaryReader
{
public:
	BinaryReader() = default;
	BinaryReader(const string& msg):_msg(msg){}

	BinaryReader& operator=(const BinaryReader& reader)
	{
		if (this != &reader)
		{
			_msg = reader._msg;
			_msg = reader._index;
		}

		return *this;
	}

	void setMsg(string& msg) { _msg = msg; _index = 0; }
	void reset() { _index = 0; }
	size_t size() { return _msg.size();}


	template<class T>
	bool readData(T& data)
	{
		if (_index + sizeof(T) > _msg.size())  // ��Ϣ����û������
			return false;
		memcpy(&ret, _msg.c_str() + _index, sizeof(T));
		_index += sizeof(T);
	}

	template<std::string>
	bool readData(std::string& data)
	{
		// TODO�������ṹ������
		return true;
	}

	~BinaryReader() =default ;

private:
	string _msg;
	int _index = 0;
};