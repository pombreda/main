#ifndef BASIS_TST_HPP_
#define BASIS_TST_HPP_

#include <basis/configure.hpp>

namespace tst {
	typedef int (*aPrintFunc)(const char*, ...);
	typedef int (*wPrintFunc)(const wchar_t*, ...);
}

namespace tst {
	ssize_t _algorithm(wPrintFunc);
	ssize_t a_string(aPrintFunc printFunc);
	ssize_t w_string(wPrintFunc printFunc);
	ssize_t _vector(aPrintFunc printFunc);
}

#include <basis/~tst/A.hpp>
#include <basis/~tst/BaseDerived.hpp>

#endif
