#pragma once

#include <string>

// 将数据 打包成二进制的包
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
	void compress(size_t len, string& out);				// 对小数据进行压缩

private:
	std::string _msg;				// 封装的包
	uint32_t _index = 0;
};
