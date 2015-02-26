#include <tests.hpp>

#include <basis/sys/logger.hpp>
#include <basis/simstd/memory>
#include <basis/tst.hpp>

#include <memory>

void test_shared_ptr()
{
	LogWarn(L"\n");

	std::shared_ptr<tst::Derived> stdd(new tst::Derived);
	LogNoise(L"sizeof(stdd): %d\n", sizeof(stdd));

	std::shared_ptr<tst::Base> stdb(new tst::Base);
	LogNoise(L"sizeof(stdb): %d\n", sizeof(stdb));

	stdb = stdd;

	simstd::shared_ptr<tst::Derived> d(new tst::Derived);
	LogNoise(L"sizeof(d): %d\n", sizeof(d));

	simstd::shared_ptr<tst::Base> b(new tst::Base);
	LogNoise(L"sizeof(b): %d\n", sizeof(b));

	b = d;
}
