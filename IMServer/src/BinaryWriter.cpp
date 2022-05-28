#include "BinaryWriter.h"
using namespace std;
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
		if (i > 0) //  说明不是 最后七位
			c |= 0x80;
		out += c;
	}
}

