#include <basis/sys/sstr.hpp>

#include <basis/simstd/string>

namespace sstr {

	ustring format(const wchar_t* fmt, va_list args)
	{
		wchar_t buf[DEFAULT_PRINTF_BUFFER];
		safe_vsnprintf(buf, lengthof(buf), fmt, args);
		return ustring(buf);
	}

}
