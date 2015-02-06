#include <tests.hpp>

#include <basis/sys/logger.hpp>
#include <basis/simstd/memory>

#include <memory>

class Base {
public:
	~Base()
	{
		LogTraceObj();
	}
	Base()
	{
		LogTraceObj();
	}
};

class Derived: public Base {
public:
	~Derived()
	{
		LogTraceObj();
	}
	Derived()
	{
		LogTraceObj();
	}
};

void test_shared_ptr()
{
	LogWarn(L"\n");

	std::shared_ptr<Derived> stdd(new Derived);
	LogNoise(L"sizeof(stdd): %d\n", sizeof(stdd));

	std::shared_ptr<Base> stdb(new Base);
	LogNoise(L"sizeof(stdb): %d\n", sizeof(stdb));

	stdb = stdd;

	simstd::shared_ptr<Derived> d(new Derived);
	LogNoise(L"sizeof(d): %d\n", sizeof(d));

	simstd::shared_ptr<Base> b(new Base);
	LogNoise(L"sizeof(b): %d\n", sizeof(b));

//	b = d;

//	LogNoise(L"uniq %s\n", uniq ? L"not empty" : L"empty");
}
