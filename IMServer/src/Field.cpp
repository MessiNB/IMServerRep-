#include "Field.h"

void Field::setValue(const char* value, size_t len)
{
	if (NULL == value || size_t == 0) {
		_isEmpty = true;
		_value.clear();
		return;
	}
	m_value.assign(value, nLen);//�滻����ָ�������ݣ��滻ԭ�е�����
	m_isnull = false;
	return;
}
