#include <basis/tst.hpp>
#include <basis/sys/logger.hpp>

namespace tst
{

	Base::~Base()
	{
		LogTraceObj();
	}
	Base::Base()
	{
		LogTraceObj();
	}

	Derived::~Derived()
	{
		LogTraceObj();
	}
	Derived::Derived()
	{
		LogTraceObj();
	}

}
