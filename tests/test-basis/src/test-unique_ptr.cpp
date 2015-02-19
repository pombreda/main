#include <tests.hpp>

#include <basis/sys/logger.hpp>
#include <basis/simstd/memory>

#include <basis/tst/A.hpp>

template<typename Type>
struct Dltr
{
	constexpr Dltr() noexcept = default;

	template<typename _Up, typename = typename std::enable_if<std::is_convertible<_Up*, Type*>::value>::type>
	Dltr(const Dltr<_Up>&) noexcept
	{
	}

	void operator ()(Type* ptr) const noexcept
		{
		LogNoise(L"[deleter called]\n");
		delete ptr;
		}
};

void test_unique_ptr()
{
	LogWarn(L"\n");

	auto deleter = [](int* ptr){
		LogNoise(L"[deleter called]\n");
		delete ptr;
	};

	simstd::unique_ptr<int, decltype(deleter)> uniq1(new int, deleter);
	LogNoise(L"sizeof(uniq1): %d\n", sizeof(uniq1));

	simstd::unique_ptr<int, Dltr<int>> uniq2(new int, Dltr<int>());
	LogNoise(L"sizeof(uniq2): %d\n", sizeof(uniq2));

	auto uniq3(simstd::make_unique<tst::A>(3));
	LogNoise(L"sizeof(uniq3): %d\n", sizeof(uniq3));

	auto uniq4(simstd::make_unique<tst::A>(4));
	LogNoise(L"sizeof(uniq4): %d\n", sizeof(uniq4));

	using simstd::swap;
	swap(uniq3, uniq4);

	LogNoise(L"uniq1 %s\n", uniq1 ? L"not empty" : L"empty");
	uniq1.reset();
	LogNoise(L"uniq %s\n", uniq1 ? L"not empty" : L"empty");
}
