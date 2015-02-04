#include <basis/configure.hpp>

#include <stdio.h>

size_t safe_vsnprintf(char* buf, size_t len, const char* format, va_list vl)
{
	buf[--len] = 0;
	int ret = ::_vsnprintf(buf, len, format, vl);
	return ret == -1 ? 0 : ret;
}
