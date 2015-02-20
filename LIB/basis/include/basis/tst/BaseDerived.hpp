#ifndef BASIS_TST_BASEDERIVED_HPP_
#define BASIS_TST_BASEDERIVED_HPP_

namespace tst {

	class Base {
	public:
		~Base();
		Base();
	};

	class Derived: public Base {
	public:
		~Derived();
		Derived();
	};

}

#endif
