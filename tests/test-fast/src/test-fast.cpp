#include <stdio.h>

#include <shared_ptr.hpp>

#include <memory>

class Base {
public:
	~Base()
	{
		printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);
	}
	Base(): i()
	{
		printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);
	}
	Base(int i): i(i)
	{
		printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);
	}

	int i;
};

class Derived: public Base {
public:
	~Derived()
	{
		printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);
	}
	Derived()
	{
		printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);
	}
};

void Base_deleter(Base* p)
{
	printf("%s:%d Base_deleter called\n", __PRETTY_FUNCTION__, __LINE__);
	delete p;
}


int main() {
	printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);

//	std::shared_ptr<int> aptr;
//	printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);
//	{
//		// create a shared_ptr that owns a Foo and a deleter
//		auto foo_p = new Base;
//		printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);
//		std::shared_ptr<Base> r(foo_p, Base_deleter);
//		printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);
//		aptr = std::shared_ptr<int>(r, &r->i); // aliasing ctor
//		printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);
//		// aptr is now pointing to an int, but managing the whole Foo
//	} // r gets destroyed (deleter not called)
//
//	// obtain pointer to the deleter:
//	auto del_p = std::get_deleter<void (*)(Base*)>(aptr);
//	printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);
//	if (del_p) {
//		printf("%s:%d shared_ptr<int> owns a deleter\n", __PRETTY_FUNCTION__, __LINE__);
//		if (*del_p == Base_deleter)
//			printf("%s:%d ...and it equals &foo_deleter\n", __PRETTY_FUNCTION__, __LINE__);
//	} else
//		printf("%s:%d The deleter of shared_ptr<int> is null!\n", __PRETTY_FUNCTION__, __LINE__);


//	std::shared_ptr<Derived> stdd(new Derived);
//	LogNoise(L"sizeof(stdd): %d\n", sizeof(stdd));

//	std::shared_ptr<Base> stdb(new Base);
//	LogNoise(L"sizeof(stdb): %d\n", sizeof(stdb));

//	stdb = stdd;

	{
		simstd1::shared_ptr<Derived> sdNull(nullptr);
		printf("%s:%d sizeof(sd):  %Iu\n", __PRETTY_FUNCTION__, __LINE__, sizeof(sdNull));
		sdNull.reset();
	}

	printf("\n%s:%d allocating der\n", __PRETTY_FUNCTION__, __LINE__);
	auto der = new Derived;
	printf("%s:%d der: %p\n", __PRETTY_FUNCTION__, __LINE__, der);

	printf("\n%s:%d allocating sd\n", __PRETTY_FUNCTION__, __LINE__);
	simstd1::shared_ptr<Derived> sd(der);
	printf("%s:%d sizeof(sd):  %Iu\n", __PRETTY_FUNCTION__, __LINE__, sizeof(sd));
	printf("%s:%d get():       %p\n", __PRETTY_FUNCTION__, __LINE__, sd.get());
	printf("%s:%d bool():      %d\n", __PRETTY_FUNCTION__, __LINE__, sd.operator bool());
	printf("%s:%d unique():    %d\n", __PRETTY_FUNCTION__, __LINE__, sd.unique());
	printf("%s:%d use_count(): %Id\n", __PRETTY_FUNCTION__, __LINE__, sd.use_count());

	printf("\n%s:%d allocating wd\n", __PRETTY_FUNCTION__, __LINE__);
	simstd1::weak_ptr<Derived> wd(sd);
	printf("%s:%d sizeof(wd):  %Iu\n", __PRETTY_FUNCTION__, __LINE__, sizeof(wd));
	printf("%s:%d use_count(): %Id\n", __PRETTY_FUNCTION__, __LINE__, wd.use_count());
	printf("%s:%d expired():   %d\n", __PRETTY_FUNCTION__, __LINE__, wd.expired());

	printf("\n%s:%d allocating sd2\n", __PRETTY_FUNCTION__, __LINE__);
	auto sd2 = wd.lock();
	printf("%s:%d sizeof(sd2): %Iu\n", __PRETTY_FUNCTION__, __LINE__, sizeof(sd2));
	printf("%s:%d get():       %p\n", __PRETTY_FUNCTION__, __LINE__, sd2.get());
	printf("%s:%d bool():      %d\n", __PRETTY_FUNCTION__, __LINE__, sd2.operator bool());
	printf("%s:%d unique():    %d\n", __PRETTY_FUNCTION__, __LINE__, sd2.unique());
	printf("%s:%d use_count(): %Id\n", __PRETTY_FUNCTION__, __LINE__, sd2.use_count());

	printf("\n%s:%d reseting sd\n", __PRETTY_FUNCTION__, __LINE__);
	sd.reset();
	printf("%s:%d get():       %p\n", __PRETTY_FUNCTION__, __LINE__, sd2.get());
	printf("%s:%d bool():      %d\n", __PRETTY_FUNCTION__, __LINE__, sd2.operator bool());
	printf("%s:%d unique():    %d\n", __PRETTY_FUNCTION__, __LINE__, sd2.unique());
	printf("%s:%d use_count(): %Id\n", __PRETTY_FUNCTION__, __LINE__, sd2.use_count());

	printf("\n%s:%d reseting sd2\n", __PRETTY_FUNCTION__, __LINE__);
	sd2.reset();
	printf("%s:%d get():       %p\n", __PRETTY_FUNCTION__, __LINE__, sd2.get());
	printf("%s:%d bool():      %d\n", __PRETTY_FUNCTION__, __LINE__, sd2.operator bool());
	printf("%s:%d unique():    %d\n", __PRETTY_FUNCTION__, __LINE__, sd2.unique());
	printf("%s:%d use_count(): %Id\n", __PRETTY_FUNCTION__, __LINE__, sd2.use_count());

	printf("\n%s:%d allocating sd3\n", __PRETTY_FUNCTION__, __LINE__);
	auto sd3 = wd.lock();
	printf("%s:%d sizeof(sd3): %Iu\n", __PRETTY_FUNCTION__, __LINE__, sizeof(sd3));
	printf("%s:%d get():       %p\n", __PRETTY_FUNCTION__, __LINE__, sd3.get());
	printf("%s:%d bool():      %d\n", __PRETTY_FUNCTION__, __LINE__, sd3.operator bool());
	printf("%s:%d unique():    %d\n", __PRETTY_FUNCTION__, __LINE__, sd3.unique());
	printf("%s:%d use_count(): %Id\n", __PRETTY_FUNCTION__, __LINE__, sd3.use_count());

//	simstd1::shared_ptr<Base> b(new Base);
//	LogNoise(L"sizeof(b): %d\n", sizeof(b));

	printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);
	return 0;
}
