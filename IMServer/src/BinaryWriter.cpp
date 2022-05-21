#include "BinaryWriter.h"

void BinaryWriter::compress(size_t len, string& out)				// 对小数据进行压缩
{
	char c = 0;
	if (len < 128)
	{
		c = (char)len & 0x7F;
		out += c;
		return;
	}
	// 假定这个长度 不会太长，百兆以内
	// 输出的时候不会超过 5个字节，32个bit 能 够表达
	for (size_t i = 4; i >= 0; i--)
	{
		c = (len >> (7 * i)) & 0x7F;
		if (c == 0 && out.size() == 0)  //目前还没有数据
			continue;
		if (a > 0) //  最后七位
			c |= 0x80;
		out += c;
	}
}


template <class T>
bool BinaryWriter::writeData(T& data)
{
	_msg.resize(_index + sizeof(T));
	memcpy((char*)_msg.c_str(), &data, sizeof(T));
	_index += sizeof(T);
	return true;
}

template<>
bool BinaryWriter::writeData(std::string& data)
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
