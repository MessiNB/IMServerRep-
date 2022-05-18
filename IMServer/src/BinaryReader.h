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
		if (_index + sizeof(T) > _msg.size())  // 消息中已没有数据
			return false;
		memcpy(&ret, _msg.c_str() + _index, sizeof(T));
		_index += sizeof(T);
	}

	~BinaryReader() =default ;

private:
	string _msg;
	int _index = 0;
};

template<>
bool BinaryReader::readData(std::string& data)
{
	memcpy((char*)data, _msg.c_str() + _index, data.size());
	_index += data.size();
	// TODO：处理结构体数据
	return true;
}