﻿#include <libbase/std.hpp>

#include <stdio.h>

namespace Base {
	bool safe_vsnprintf(PSTR buf, size_t len, PCSTR format, va_list vl)
	{
		buf[--len] = 0;
		return static_cast<unsigned>(::_vsnprintf(buf, len, format, vl)) <= len;
	}

	bool safe_snprintf(PSTR buff, size_t len, PCSTR format, ...)
	{
		Base::Va_list vl;
		va_start(vl, format);
		return safe_vsnprintf(buff, len, format, vl);
	}
}