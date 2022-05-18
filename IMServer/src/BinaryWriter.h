#include <string>

class BinaryWriter
{
public:
	BinaryWriter() = default;
	~BinaryWriter() = default;

	template<class T>
	bool writeData(T& data)
	{
		_msg.resize(_index + sizeof(T));
		memcpy((char*)_msg.c_str(), &data, sizeof(T));
		_index += sizeof(T);
		return true;
	}
protected:
	void cpmpress(size_t len, string& out)				// 对小数据进行压缩
	{
		size_t  out_len = 0;
		if (len < 128)
		{
			out_len += (char)len;
			return;
		}


	}
private:
	std::string _msg;
	uint32_t _index = 0;
};

template<>
bool BinaryWriter::writeData(std::string& data)
{
	if (data.size() > 0)
	{

	}
}