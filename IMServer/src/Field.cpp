#include "Field.h"

void Field::setValue(const char* value, size_t len)
{
	if (NULL == value || size_t == 0) {
		_isEmpty = true;
		_value.clear();
		return;
	}
	m_value.assign(value, nLen);//替换，用指定的内容，替换原有的内容
	m_isnull = false;
	return;
}
