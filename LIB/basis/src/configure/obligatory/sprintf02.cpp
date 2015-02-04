#include <basis/configure.hpp>

#include <stdio.h>

size_t safe_snprintf(char* buff, size_t len, const char* format, ...)
{
	Va_list vl;
	va_start(vl, format);
	return safe_vsnprintf(buff, len, format, vl);
}
