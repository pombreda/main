#include <tests.hpp>

#include <basis/sys/logger.hpp>
#include <basis/simstd/memory>

void test_unique_ptr()
{
	LogWarn(L"\n");

	auto deleter = [](int* ptr){
		LogNoise(L"[deleter called]\n");
		delete ptr;
	};

	simstd::unique_ptr<int, decltype(deleter)> uniq(new int, deleter);
	LogNoise(L"sizeof(uniq): %d\n", sizeof(uniq));

	LogNoise(L"uniq %s\n", uniq ? L"not empty" : L"empty");
	uniq.reset();
	LogNoise(L"uniq %s\n", uniq ? L"not empty" : L"empty");
}
