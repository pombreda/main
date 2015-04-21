#include <basis/sys/crt.hpp>
#include <stdio.h>

extern "C"
{
	extern const crt::Function __CTOR_LIST__[1];
	extern const crt::Function __DTOR_LIST__[1];
}

namespace
{
	const ssize_t MAX_ATEXITLIST_ENTRIES = 64;

	LONG atExitIndex(0);

	crt::Function atExitArray[MAX_ATEXITLIST_ENTRIES];
}

namespace crt
{
	void invoke_crt_functions(const Function* pf, ptrdiff_t step)
	{
		TraceFunc();
		for (; *pf; pf += step) {
			if (reinterpret_cast<intptr_t>(*pf) == static_cast<intptr_t>(-1)) {
				continue;
			} else {
				TraceFormatFunc("%p->%p\n", pf, *pf);
				(*pf)();
			}
		}
		TraceFunc();
	}

	void invoke_ctors()
	{
		TraceFormatFunc("__CTOR_LIST__: %p\n", __CTOR_LIST__);
		invoke_crt_functions(__CTOR_LIST__, 1);
		TraceFunc();
	}

	void invoke_dtors()
	{
		TraceFormatFunc("__DTOR_LIST__: %p\n", __DTOR_LIST__);
		invoke_crt_functions(__DTOR_LIST__, 1);
		TraceFunc();
	}

	void init_atexit()
	{
		TraceFunc();
		for (ssize_t i = 0; i < MAX_ATEXITLIST_ENTRIES; ++i)
			atExitArray[i] = reinterpret_cast<Function>(-1);
		atexit(reinterpret_cast<Function>(0));
		atexit(&invoke_dtors);

		invoke_ctors();
		TraceFunc();
	}

	void invoke_atexit()
	{
		TraceFunc();
		invoke_crt_functions(&atExitArray[MAX_ATEXITLIST_ENTRIES - 1], -1);
		TraceFunc();
	}

	int atexit(Function pf)
	{
		TraceFunc();
		auto ind = InterlockedExchangeAdd(&atExitIndex, 1);

		TraceFormatFunc("func: %p, index: %d\n", pf, (int)ind);
		if (ind < MAX_ATEXITLIST_ENTRIES)
		{
			atExitArray[ind] = pf;
			TraceFunc();
			return 0;
		}
		TraceFunc();
		return -1;
	}

	void cxa_pure_virtual()
	{
		printf("%s():%d pure virtual function called\n", __FUNCTION__, __LINE__);
//		::abort_message("pure virtual method called");
	}
}
