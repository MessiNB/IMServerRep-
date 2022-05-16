#include "Field.h"

void Field::setValue(const char* value)
{
	if (NULL == value) {
		_isEmpty = true;
		_value.clear();
		return;
	}
	_isEmpty = false;
	_value = value;
	return;
}
