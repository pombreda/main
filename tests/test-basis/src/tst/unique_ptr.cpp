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

	int i = int();
};

ssize_t tst::_unique_ptr()
{
	TestFuncPlaceFormat("BEGIN\n");

	auto deleter = [](Base1* ptr){
		TestFuncPlaceFormat("[deleter called]\n");
		delete ptr;
	};

	simstd::unique_ptr<Derived1, decltype(deleter)> uniq1(new Derived1, deleter);
	   std::unique_ptr<Derived1, decltype(deleter)> uniq1s(new Derived1, deleter);
	TestFuncPlaceFormat("sizeof(uniq1): %d\n", sizeof(uniq1));
	TestFuncPlaceFormat("sizeof(uniq1s): %d\n", sizeof(uniq1s));

	simstd::unique_ptr<Base1, Dltr<Base1>> uniq2(new Base1, Dltr<Base1>());
	   std::unique_ptr<Base1, Dltr<Base1>> uniq2s(new Base1, Dltr<Base1>());
	TestFuncPlaceFormat("sizeof(uniq2): %d\n", sizeof(uniq2));
	TestFuncPlaceFormat("sizeof(uniq2s): %d\n", sizeof(uniq2s));

	auto uniq3(simstd::make_unique<tst::A>(3));
	TestFuncPlaceFormat("sizeof(uniq3): %d, get: %p\n", sizeof(uniq3), uniq3.get());

	auto uniq4(simstd::make_unique<tst::A>(4));
	TestFuncPlaceFormat("sizeof(uniq4): %d, get: %p\n", sizeof(uniq4), uniq4.get());

	using simstd::swap;
	swap(uniq3, uniq4);

	TestFuncPlaceFormat("uniq1 %s\n", uniq1 ? "not empty" : "empty");
	uniq1.reset();
	TestFuncPlaceFormat("uniq1 %s\n", uniq1 ? "not empty" : "empty");

	TestFuncPlaceFormat("END\n");
	return 0;
}
