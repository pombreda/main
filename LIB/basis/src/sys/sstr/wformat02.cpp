#include <basis/sys/sstr.hpp>

#include <basis/simstd/string>

namespace sstr {

	ustring format(const wchar_t* fmt, ...)
	{
		Va_list args;
		va_start(args, fmt);
		return format(fmt, args);
	}

}
