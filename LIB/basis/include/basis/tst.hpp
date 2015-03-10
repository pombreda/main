#ifndef BASIS_TST_HPP_
#define BASIS_TST_HPP_

#include <basis/configure.hpp>

#define TestFuncFormat(format, ...) console::printf(format, ##__VA_ARGS__)
#define TestFuncPlace() console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__)
#define TestFuncPlaceFormat(format, ...) console::printf("%s:%d " format, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)

namespace tst {
	typedef int (*aPrintFunc)(const char*, ...);
	typedef int (*wPrintFunc)(const wchar_t*, ...);
}

namespace tst {
	ssize_t _algorithm();
	ssize_t _list();
	ssize_t a_string(aPrintFunc printFunc);
	ssize_t w_string(wPrintFunc printFunc);
	ssize_t _shared_ptr();
	ssize_t _unique_ptr();
	ssize_t _vector();
}

#include <basis/~tst/A.hpp>
#include <basis/~tst/BaseDerived.hpp>

#endif
