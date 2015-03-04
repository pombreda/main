#ifndef BASIS_TST_BASEDERIVED_HPP_
#define BASIS_TST_BASEDERIVED_HPP_

namespace tst {

	class Base1 {
	public:
		virtual ~Base1();
		Base1();
	private:
		ssize_t i = ssize_t();
	};

	class Base2 {
	public:
		virtual ~Base2();
		Base2();
	private:
		ssize_t i = ssize_t();
	};

	class Derived1: public Base1 {
	public:
		~Derived1();
		Derived1();
	private:
		ssize_t i = ssize_t();
	};

	class Derived2: public Base1, public Base2 {
	public:
		~Derived2();
		Derived2();
	private:
		ssize_t i = ssize_t();
	};
}

#endif
