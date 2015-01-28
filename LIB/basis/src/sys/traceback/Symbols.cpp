#include <basis/os/dbghelp.hpp>
#include <basis/sys/traceback.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/memory.hpp>
#include <basis/sys/totext.hpp>

#include "Bfd.hpp"

namespace traceback {

	LogRegisterLocal(L"traceback");

	struct DebugSymbols {
		static DebugSymbols& inst(const wchar_t* path = nullptr)
		{
			static DebugSymbols instance(path);
			return instance;
		}

#if defined(__GNUC__) && defined(DEBUG)
		Bfd bfd;
#endif

	private:
		~DebugSymbols()
		{
			bool ret = os::Dbghelp_dll::inst().SymCleanup(::GetCurrentProcess());
			LogTraceIf(ret);
			LogErrorIf(!ret, L"%s\n", totext::api_error().c_str());
			UNUSED(ret);
		}

		DebugSymbols(const wchar_t* path)
		{
			os::Dbghelp_dll::inst().SymSetOptions(os::Dbghelp_dll::inst().SymGetOptions() | /*SYMOPT_DEFERRED_LOADS | */SYMOPT_FAIL_CRITICAL_ERRORS | SYMOPT_LOAD_LINES);
			bool ret = os::Dbghelp_dll::inst().SymInitializeW(::GetCurrentProcess(), path, TRUE);
			LogNoiseIf(ret, L"['%s']\n", path);
			LogErrorIf(!ret, L"['%s'] -> %s\n", path, totext::api_error().c_str());
			UNUSED(ret);
		}
	};

#if defined(__GNUC__) && defined(DEBUG)
	void bfd_fill(const char*& file, const char*& func, size_t& line, const wchar_t* image, void* address)
	{
		DebugSymbols::inst().bfd.fill(file, func, line, image, address);
	}
#endif

}

void traceback::init(const wchar_t* path)
{
	DebugSymbols::inst(path);
}
