﻿#include <basis/sys/console.hpp>

namespace console {

	int wprintf(const wchar_t* format, ...)
	{
		Va_list vl;
		va_start(vl, format);
		return vprintf(Handle::OUTPUT, format, vl);
	}

}
