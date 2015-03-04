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

	Base2::~Base2()
	{
		LogTraceObj();
	}
	Base2::Base2()
	{
		LogTraceObj();
	}

	Derived1::~Derived1()
	{
		LogTraceObj();
	}
	Derived1::Derived1()
	{
		LogTraceObj();
	}

	Derived2::~Derived2()
	{
		LogTraceObj();
	}
	Derived2::Derived2()
	{
		LogTraceObj();
	}

}
