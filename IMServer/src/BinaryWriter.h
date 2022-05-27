#pragma once

#include <string>

// ������ ����ɶ����Ƶİ�
class BinaryWriter
{
public:
	BinaryWriter() = default;
	~BinaryWriter() = default;


	template <class T>
	bool writeData(T& data)
	{
		_msg.resize(_index + sizeof(T));
		memcpy((char*)_msg.c_str(), &data, sizeof(T));
		_index += sizeof(T);
		return true;
	}

	bool writeData(std::string& data)
	{

		std::string out;
		compress(data.size(), out);
		_msg.append(out.c_str(), out.size());
		_index += out.size();

		if (data.size() > 0)
		{
			_msg.append(data.c_str(), data.size());
			_index += data.size();
		}

		return false;
	}

	size_t size() { return _index; };
	std::string getMsg() { return _msg; };
	void clear() { _index = 0, _msg.clear(); }
protected:
	void compress(size_t len, string& out);				// ��С���ݽ���ѹ��

private:
	std::string _msg;				// ��װ�İ�
	uint32_t _index = 0;
};
