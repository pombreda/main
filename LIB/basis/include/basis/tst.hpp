#ifndef BASIS_TST_HPP_
#define BASIS_TST_HPP_

#include <basis/configure.hpp>
#include <basis/sys/console.hpp>

#define TestFuncFormat(format, ...) console::printf(format, ##__VA_ARGS__)
#define TestFuncPlace() console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__)
#define TestFuncPlaceFormat(format, ...) console::printf("%s:%d " format, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)

namespace tst
{
	ssize_t _algorithm();
	ssize_t _list();
	ssize_t _Scheduler();
	ssize_t _shared_ptr();
	ssize_t _string();
	ssize_t _unique_ptr();
	ssize_t _vector();

	ssize_t _sync();
}

#include <basis/~tst/A.hpp>
#include <basis/~tst/BaseDerived.hpp>

#endif
