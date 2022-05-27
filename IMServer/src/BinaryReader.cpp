#include "BinaryReader.h"
#include <cstring>
BinaryReader::operator=(const BinaryReader& reader)
{
		if (this != &reader)
		{
			_msg = reader._msg;
			_msg = reader._index;
		}
		return *this;
}
