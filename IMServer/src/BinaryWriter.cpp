#include "BinaryWriter.h"
using namespace std;
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
		if (i > 0) //  ˵������ �����λ
			c |= 0x80;
		out += c;
	}
}

