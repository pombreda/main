#include <basis/sys/crt.hpp>
#include <basis/sys/console.hpp>

namespace crt
{
	void assrt(const char* message, const char* file, int line, const char* func)
	{
		console::printf(console::Handle::ERR, "Assertion failed <%u> at %s line %d: '%s' (%s)\n", ::GetCurrentThreadId(), file, line, message, func);
		::TerminateProcess(::GetCurrentProcess(), static_cast<UINT>(-1));
	}

	void assrt(const wchar_t* message, const char* file, int line, const char* func)
	{
		console::printf(console::Handle::ERR, L"Assertion failed <%u> at %S line %d: '%s' (%S)\n", ::GetCurrentThreadId(), file, line, message, func);
		::TerminateProcess(::GetCurrentProcess(), static_cast<UINT>(-1));
	}
}
