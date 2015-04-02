#include <basis/tst.hpp>
#include <basis/sys/logger.hpp>

namespace tst
{

	ssize_t Base1::objects = 0;
	Base1::~Base1()
	{
		LogTraceObjLn();
		--objects;
	}
	Base1::Base1()
	{
		LogTraceObjLn();
		++objects;
	}
	ssize_t Base1::get_value() const
	{
		LogTraceObjLn();
		return i;
	}

	Base2::~Base2()
	{
		LogTraceObjLn();
	}
	Base2::Base2()
	{
		LogTraceObjLn();
	}
	ssize_t Base2::get_value() const
	{
		LogTraceObjLn();
		return i;
	}

	Derived1::~Derived1()
	{
		LogTraceObjLn();
	}
	Derived1::Derived1()
	{
		LogTraceObjLn();
	}
	ssize_t Derived1::get_value() const
	{
		LogTraceObjLn();
		return i;
	}

	Derived2::~Derived2()
	{
		LogTraceObjLn();
	}
	Derived2::Derived2()
	{
		LogTraceObjLn();
	}
	ssize_t Derived2::get_value() const
	{
		LogTraceObjLn();
		return i;
	}

}
