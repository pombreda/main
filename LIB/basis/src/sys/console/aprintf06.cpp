﻿
#include <basis/sys/console.hpp>
#include <basis/sys/cstr.hpp>
#include <basis/sys/memory.hpp>

namespace {
	const size_t DEFAULT_PRINTF_BUFFER = 8 * 1024;
}

namespace console {

	size_t vprintf_var(Handle hnd, const char* format, va_list vl)
	{
		typedef memory::heap::Default heap_type;
		typedef char char_type;

		char_type* buff = nullptr;
		size_t len = DEFAULT_PRINTF_BUFFER;
		do {
			buff = static_cast<char_type*>(heap_type::realloc(buff, len * sizeof(char_type)));
		} while (buff && !safe_vsnprintf(buff, len, format, vl) && (len *= 2));

		auto ret = puts(buff, cstr::length(buff), hnd);
		heap_type::free(buff);

		return ret;
	}

	size_t vprintf(Handle hnd, const char* format, va_list vl)
	{
		{
			char buff[DEFAULT_PRINTF_BUFFER / 2];
			if (safe_vsnprintf(buff, lengthof(buff), format, vl))
				return puts(buff, cstr::length(buff), hnd);
		}
		return vprintf_var(hnd, format, vl);
	}

}