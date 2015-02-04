#include <basis/configure.hpp>

size_t safe_snprintf(wchar_t* buff, size_t len, const wchar_t* format, ...)
{
	Va_list vl;
	va_start(vl, format);
	return safe_vsnprintf(buff, len, format, vl);
}
