#include <basis/sys/sstr.hpp>

#include <basis/simstd/string>

namespace sstr {

	astring format(const char* fmt, ...)
	{
		Va_list args;
		va_start(args, fmt);
		return format(fmt, args);
	}

	astring format(const char* fmt, va_list args)
	{
		char buf[DEFAULT_PRINTF_BUFFER];
		safe_vsnprintf(buf, lengthof(buf), fmt, args);
		return astring(buf);
	}

}
