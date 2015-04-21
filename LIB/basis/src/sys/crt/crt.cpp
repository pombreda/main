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
		TraceFuncLn();
		for (; *pf; pf += step) {
			if (reinterpret_cast<intptr_t>(*pf) == static_cast<intptr_t>(-1)) {
				continue;
			} else {
				TraceFunc("%p->%p\n", pf, *pf);
				(*pf)();
			}
		}
		TraceFuncLn();
	}

	void invoke_ctors()
	{
		TraceFunc("__CTOR_LIST__: %p\n", __CTOR_LIST__);
		invoke_crt_functions(__CTOR_LIST__, 1);
		TraceFuncLn();
	}

	void invoke_dtors()
	{
		TraceFunc("__DTOR_LIST__: %p\n", __DTOR_LIST__);
		invoke_crt_functions(__DTOR_LIST__, 1);
		TraceFuncLn();
	}

	void init_atexit()
	{
		TraceFuncLn();
		for (ssize_t i = 0; i < MAX_ATEXITLIST_ENTRIES; ++i)
			atExitArray[i] = reinterpret_cast<Function>(-1);
		atexit(reinterpret_cast<Function>(0));
		atexit(&invoke_dtors);

		invoke_ctors();
		TraceFuncLn();
	}

	void invoke_atexit()
	{
		TraceFuncLn();
		invoke_crt_functions(&atExitArray[MAX_ATEXITLIST_ENTRIES - 1], -1);
		TraceFuncLn();
	}

	int atexit(Function pf)
	{
		TraceFuncLn();
		auto ind = InterlockedExchangeAdd(&atExitIndex, 1);

		TraceFunc("func: %p, index: %d\n", pf, (int)ind);
		if (ind < MAX_ATEXITLIST_ENTRIES)
		{
			atExitArray[ind] = pf;
			TraceFuncLn();
			return 0;
		}
		TraceFuncLn();
		return -1;
	}

	void cxa_pure_virtual()
	{
		printf("%s():%d pure virtual function called\n", __FUNCTION__, __LINE__);
//		::abort_message("pure virtual method called");
	}
}
