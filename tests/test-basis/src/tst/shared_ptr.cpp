#include <basis/tst.hpp>
#include <basis/sys/console.hpp>

#include <basis/simstd/memory>
#include <memory>

namespace tst
{
	void A_deleter(A* p)
	{
		TestFuncPlaceFormat("A_deleter called\n");
		delete p;
	}

	void A_deleter2(A* p)
	{
		TestFuncPlaceFormat("A_deleter called\n");
		delete p;
	}

	void Base_deleter(Base1* p)
	{
		TestFuncPlaceFormat("Base1_deleter called\n");
		delete p;
	}

	struct Good: simstd::enable_shared_from_this<Good>
	{
		simstd::shared_ptr<Good> getptr() {
			return shared_from_this();
		}
		~Good() {TestFuncPlaceFormat("called\n");}
	};

	struct Bad
	{
		simstd::shared_ptr<Bad> getptr() {
			return simstd::shared_ptr<Bad>(this);
		}
		~Bad() {TestFuncPlaceFormat("called\n");}
	};
}

ssize_t tst::_shared_ptr()
{
	TestFuncPlaceFormat("begin\n");

	{
		auto der2 = simstd::allocate_shared<Derived2>(simstd::allocator<Derived2>());
		simstd::shared_ptr<Base1> bas1(der2);
		simstd::shared_ptr<Base2> bas2(der2);
		TestFuncPlaceFormat("get(der2):      %p\n",  der2.get());
		TestFuncPlaceFormat("get(bas1):      %p\n",  bas1.get());
		TestFuncPlaceFormat("get(bas2):      %p\n",  bas2.get());
		TestFuncPlaceFormat("sizeof(*der2):  %Iu\n", sizeof(*der2));
		TestFuncPlaceFormat("sizeof(*bas1):  %Iu\n", sizeof(*bas1));
		TestFuncPlaceFormat("sizeof(*bas2):  %Iu\n", sizeof(*bas2));

		auto der2i = simstd::shared_ptr<ssize_t>(der2, &der2->i);
		auto der2i1 = simstd::shared_ptr<ssize_t>(der2, &der2->Base1::i);
		auto der2i2 = simstd::shared_ptr<ssize_t>(der2, &der2->Base2::i);
		auto der2i21 = simstd::shared_ptr<ssize_t>(der2, &der2->Derived2::i);
		auto bas1i = simstd::shared_ptr<ssize_t>(bas1, &bas1->i);
		auto bas2i = simstd::shared_ptr<ssize_t>(bas2, &bas2->i);
		TestFuncPlaceFormat("get(&der2i->i):      %p\n",  der2i.get());
		TestFuncPlaceFormat("get(&der2i1->i):     %p\n",  der2i1.get());
		TestFuncPlaceFormat("get(&der2i2->i):     %p\n",  der2i2.get());
		TestFuncPlaceFormat("get(&der2i21->i):    %p\n",  der2i21.get());
		TestFuncPlaceFormat("get(&bas1i->i):      %p\n",  bas1i.get());
		TestFuncPlaceFormat("get(&bas2i->i):      %p\n",  bas2i.get());

//		Derived2* qwe = new Derived2;
//		TestFuncPlaceFormat("qwe):               %p\n",  qwe);
//		TestFuncPlaceFormat("qwe(Base1)):        %p\n",  static_cast<Base1*>(qwe));
//		TestFuncPlaceFormat("qwe(Base2)):        %p\n",  static_cast<Base2*>(qwe));
//		TestFuncPlaceFormat("&qwe->Base1::i):    %p\n",  &qwe->Base1::i);
//		TestFuncPlaceFormat("&qwe->Base2::i):    %p\n",  &qwe->Base2::i);
//		TestFuncPlaceFormat("&qwe->Derived2::i): %p\n",  &qwe->Derived2::i);
//		TestFuncPlaceFormat("&qwe->i):           %p\n",  &qwe->i);
	}

	{
		auto sdAuto2 = simstd::allocate_shared<Derived1>(simstd::allocator<Derived1>());
		TestFuncPlaceFormat("sizeof(sdAuto2):  %Iu\n", sizeof(sdAuto2));
		TestFuncPlaceFormat("get():            %p\n",  sdAuto2.get());
		sdAuto2.reset();
	}

	{
		auto sdAuto1 = simstd::make_shared<Derived1>();
		TestFuncPlaceFormat("sizeof(sdAuto1):  %Iu\n", sizeof(sdAuto1));
		TestFuncPlaceFormat("get():            %p\n",  sdAuto1.get());
		sdAuto1.reset();
	}

	{
		simstd::shared_ptr<int> aptr1, aptr2;
		TestFuncPlace();
		{
			// create a shared_ptr that owns a A and a A_deleter
			auto foo_p = new A;
			simstd::shared_ptr<A> r(foo_p, A_deleter);
			TestFuncPlaceFormat("r: %p\n", r.get());
			aptr1 = simstd::shared_ptr<int>(r, &r->j); // aliasing ctor
			TestFuncPlaceFormat("&r->j: %p\n", aptr1.get());
			aptr2 = simstd::shared_ptr<int>(r, &r->i); // aliasing ctor
			TestFuncPlaceFormat("&r->i: %p\n", aptr2.get());
			TestFuncPlaceFormat("sizeof(r): %Iu, r.use_count: %Iu\n", sizeof(r), r.use_count());
			// aptr1 is now pointing to an int, but managing the whole A
		} // r gets destroyed (deleter not called)

		// obtain pointer to the deleter:
		auto del_p = simstd::get_deleter<void(*)(A*)>(aptr1);
		TestFuncPlace();
		if (del_p) {
			TestFuncPlaceFormat("shared_ptr<int> owns a deleter\n");
			if (*del_p == A_deleter)
				TestFuncPlaceFormat("...and it equals &A_deleter\n");
		} else
			TestFuncPlaceFormat("The deleter of shared_ptr<int> is null!\n");
	}

	{
		std::shared_ptr<Derived1> stdd(new Derived1);
		TestFuncPlaceFormat("sizeof(stdd): %Iu\n", sizeof(stdd));

		std::shared_ptr<Base1> stdb(new Base1);
		TestFuncPlaceFormat("sizeof(stdb): %Iu\n", sizeof(stdb));

		stdb = stdd;
	}

	{
		simstd::shared_ptr<Derived1> d(new Derived1);
		TestFuncPlaceFormat("sizeof(d): %Iu\n", sizeof(d));

		simstd::shared_ptr<Base1> b(new Base1);
		TestFuncPlaceFormat("sizeof(b): %Iu\n", sizeof(b));

		b = d;
	}

	{
		simstd::shared_ptr<Derived1> sdNull(nullptr);
		TestFuncPlaceFormat("sizeof(sdNull):  %Iu\n", sizeof(sdNull));
		TestFuncPlaceFormat("get():           %p\n",  sdNull.get());
		sdNull.reset();
	}

	{
		simstd::shared_ptr<Base1> sdb(new Derived1);
		TestFuncPlaceFormat("sizeof(sdb):  %Iu\n", sizeof(sdb));
		TestFuncPlaceFormat("get():       %p\n",   sdb.get());
		sdb.reset();
	}

	{
		simstd::shared_ptr<Derived1> sdd(new Derived1);
		TestFuncPlaceFormat("sizeof(sdd):  %Iu\n", sizeof(sdd));
		TestFuncPlaceFormat("get():        %p\n",  sdd.get());
		sdd.reset();
	}

	{
		auto deleter = [](const Derived1* ptr){
			TestFuncPlaceFormat("[deleter called]: %p\n", ptr);
			delete ptr;
		};

		simstd::shared_ptr<Derived1> sdDeleter(new Derived1, deleter);
		TestFuncPlaceFormat("sizeof(sdDeleter):  %Iu\n", sizeof(sdDeleter));
		TestFuncPlaceFormat("get():              %p\n",  sdDeleter.get());
		sdDeleter.reset();
	}

	{
		// Good: the two shared_ptr's share the same object
		simstd::shared_ptr<Good> gp1(new Good);
		simstd::shared_ptr<Good> gp2 = gp1->getptr();
		TestFuncPlaceFormat("gp2.use_count():  %Iu\n", gp2.use_count());

		// Bad, each shared_ptr thinks it's the only owner of the object
		simstd::shared_ptr<Bad> bp1(new Bad);
//		simstd::shared_ptr<Bad> bp2 = bp1->getptr(); // will crush cause delete twice
		TestFuncPlaceFormat("bp1.use_count():  %Iu\n", bp1.use_count());
	}

	TestFuncPlaceFormat("\nallocating der\n");
	auto der = new Derived1;
	TestFuncPlaceFormat("der: %p\n", der);

	TestFuncPlaceFormat("\nallocating sd\n");
	simstd::shared_ptr<Derived1> sd(der);
	TestFuncPlaceFormat("sizeof(sd):  %Iu\n", sizeof(sd));
	TestFuncPlaceFormat("get():       %p\n",  sd.get());
	TestFuncPlaceFormat("bool():      %d\n",  sd.operator bool());
	TestFuncPlaceFormat("unique():    %d\n",  sd.unique());
	TestFuncPlaceFormat("use_count(): %Iu\n", sd.use_count());

	TestFuncPlaceFormat("\nallocating wd\n");
	simstd::weak_ptr<Derived1> wd(sd);
	TestFuncPlaceFormat("sizeof(wd):  %Iu\n", sizeof(wd));
	TestFuncPlaceFormat("use_count(): %Iu\n", wd.use_count());
	TestFuncPlaceFormat("expired():   %d\n", wd.expired());

	TestFuncPlaceFormat("\nallocating sd2\n");
	auto sd2 = wd.lock();
	TestFuncPlaceFormat("sizeof(sd2): %Iu\n", sizeof(sd2));
	TestFuncPlaceFormat("get():       %p\n", sd2.get());
	TestFuncPlaceFormat("bool():      %d\n", sd2.operator bool());
	TestFuncPlaceFormat("unique():    %d\n", sd2.unique());
	TestFuncPlaceFormat("use_count(): %Iu\n", sd2.use_count());

	TestFuncPlaceFormat("\nreseting sd\n");
	sd.reset();
	TestFuncPlaceFormat("get():       %p\n", sd2.get());
	TestFuncPlaceFormat("bool():      %d\n", sd2.operator bool());
	TestFuncPlaceFormat("unique():    %d\n", sd2.unique());
	TestFuncPlaceFormat("use_count(): %Iu\n", sd2.use_count());

	TestFuncPlaceFormat("\nreseting sd2\n");
	sd2.reset();
	TestFuncPlaceFormat("get():       %p\n", sd2.get());
	TestFuncPlaceFormat("bool():      %d\n", sd2.operator bool());
	TestFuncPlaceFormat("unique():    %d\n", sd2.unique());
	TestFuncPlaceFormat("use_count(): %Iu\n", sd2.use_count());

	TestFuncPlaceFormat("\nallocating sd3\n");
	auto sd3 = wd.lock();
	TestFuncPlaceFormat("sizeof(sd3): %Iu\n", sizeof(sd3));
	TestFuncPlaceFormat("get():       %p\n", sd3.get());
	TestFuncPlaceFormat("bool():      %d\n", sd3.operator bool());
	TestFuncPlaceFormat("unique():    %d\n", sd3.unique());
	TestFuncPlaceFormat("use_count(): %Iu\n", sd3.use_count());

	TestFuncPlaceFormat("end\n");
	return 0;
}
