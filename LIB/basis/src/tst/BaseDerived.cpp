#include <basis/tst.hpp>
#include <basis/sys/logger.hpp>

namespace tst
{

	Base1::~Base1()
	{
		LogTraceObj();
	}
	Base1::Base1()
	{
		LogTraceObj();
	}
	ssize_t Base1::get_value() const
	{
		LogTraceObj();
		return i;
	}

	Base2::~Base2()
	{
		LogTraceObj();
	}
	Base2::Base2()
	{
		LogTraceObj();
	}
	ssize_t Base2::get_value() const
	{
		LogTraceObj();
		return i;
	}

	Derived1::~Derived1()
	{
		LogTraceObj();
	}
	Derived1::Derived1()
	{
		LogTraceObj();
	}
	ssize_t Derived1::get_value() const
	{
		LogTraceObj();
		return i;
	}

	Derived2::~Derived2()
	{
		LogTraceObj();
	}
	Derived2::Derived2()
	{
		LogTraceObj();
	}
	ssize_t Derived2::get_value() const
	{
		LogTraceObj();
		return i;
	}

}
