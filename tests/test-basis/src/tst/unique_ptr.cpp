#include <basis/tst.hpp>
#include <basis/sys/console.hpp>

#include <basis/simstd/memory>
#include <memory>

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
		TestFuncPlaceFormat("[deleter called]\n");
		delete ptr;
	}
};

ssize_t tst::_unique_ptr()
{
	TestFuncPlaceFormat("\n");

	auto deleter = [](int* ptr){
		TestFuncPlaceFormat("[deleter called]\n");
		delete ptr;
	};

	simstd::unique_ptr<int, decltype(deleter)> uniq1(new int, deleter);
	TestFuncPlaceFormat("sizeof(uniq1): %d\n", sizeof(uniq1));

	simstd::unique_ptr<int, Dltr<int>> uniq2(new int, Dltr<int>());
	TestFuncPlaceFormat("sizeof(uniq2): %d\n", sizeof(uniq2));

	auto uniq3(simstd::make_unique<tst::A>(3));
	TestFuncPlaceFormat("sizeof(uniq3): %d\n", sizeof(uniq3));

	auto uniq4(simstd::make_unique<tst::A>(4));
	TestFuncPlaceFormat("sizeof(uniq4): %d\n", sizeof(uniq4));

	using simstd::swap;
	swap(uniq3, uniq4);

	TestFuncPlaceFormat("uniq1 %s\n", uniq1 ? L"not empty" : L"empty");
	uniq1.reset();
	TestFuncPlaceFormat("uniq %s\n", uniq1 ? L"not empty" : L"empty");

	return 0;
}
