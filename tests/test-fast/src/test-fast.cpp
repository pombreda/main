#include <stdio.h>

#include <shared_ptr.hpp>

#include <memory>

class Base {
public:
	~Base()
	{
		printf("%s():%d\n", __PRETTY_FUNCTION__, __LINE__);
	}
	Base(): i()
	{
		printf("%s():%d\n", __PRETTY_FUNCTION__, __LINE__);
	}
	Base(int i): i(i)
	{
		printf("%s():%d\n", __PRETTY_FUNCTION__, __LINE__);
	}

	int i;
};

class Derived: public Base {
public:
	~Derived()
	{
		printf("%s():%d\n", __PRETTY_FUNCTION__, __LINE__);
	}
	Derived()
	{
		printf("%s():%d\n", __PRETTY_FUNCTION__, __LINE__);
	}
};

void Base_deleter(Base* p)
{
	printf("%s():%d Base_deleter called\n", __PRETTY_FUNCTION__, __LINE__);
	delete p;
}


int main() {
	printf("%s():%d\n", __PRETTY_FUNCTION__, __LINE__);

	std::shared_ptr<int> aptr;
	printf("%s():%d\n", __PRETTY_FUNCTION__, __LINE__);
	{
		// create a shared_ptr that owns a Foo and a deleter
		auto foo_p = new Base;
		printf("%s():%d\n", __PRETTY_FUNCTION__, __LINE__);
		std::shared_ptr<Base> r(foo_p, Base_deleter);
		printf("%s():%d\n", __PRETTY_FUNCTION__, __LINE__);
		aptr = std::shared_ptr<int>(r, &r->i); // aliasing ctor
		printf("%s():%d\n", __PRETTY_FUNCTION__, __LINE__);
		// aptr is now pointing to an int, but managing the whole Foo
	} // r gets destroyed (deleter not called)

	// obtain pointer to the deleter:
	auto del_p = std::get_deleter<void (*)(Base*)>(aptr);
	printf("%s():%d\n", __PRETTY_FUNCTION__, __LINE__);
	if (del_p) {
		printf("%s():%d shared_ptr<int> owns a deleter\n", __PRETTY_FUNCTION__, __LINE__);
		if (*del_p == Base_deleter)
			printf("%s():%d ...and it equals &foo_deleter\n", __PRETTY_FUNCTION__, __LINE__);
	} else
		printf("%s():%d The deleter of shared_ptr<int> is null!\n", __PRETTY_FUNCTION__, __LINE__);


//	std::shared_ptr<Derived> stdd(new Derived);
//	LogNoise(L"sizeof(stdd): %d\n", sizeof(stdd));

//	std::shared_ptr<Base> stdb(new Base);
//	LogNoise(L"sizeof(stdb): %d\n", sizeof(stdb));

//	stdb = stdd;

	simstd1::shared_ptr<Derived> d(new Derived);
	printf("%s():%d sizeof(d): %Iu\n", __PRETTY_FUNCTION__, __LINE__, sizeof(d));

//	simstd1::shared_ptr<Base> b(new Base);
//	LogNoise(L"sizeof(b): %d\n", sizeof(b));

	printf("%s():%d\n", __PRETTY_FUNCTION__, __LINE__);
	return 0;
}
