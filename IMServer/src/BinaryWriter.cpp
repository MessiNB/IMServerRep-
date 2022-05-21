#include "BinaryWriter.h"

void BinaryWriter::compress(size_t len, string& out)				// ��С���ݽ���ѹ��
{
	char c = 0;
	if (len < 128)
	{
		c = (char)len & 0x7F;
		out += c;
		return;
	}
	// �ٶ�������� ����̫������������
	// �����ʱ�򲻻ᳬ�� 5���ֽڣ�32��bit �� �����
	for (size_t i = 4; i >= 0; i--)
	{
		c = (len >> (7 * i)) & 0x7F;
		if (c == 0 && out.size() == 0)  //Ŀǰ��û������
			continue;
		if (a > 0) //  �����λ
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
