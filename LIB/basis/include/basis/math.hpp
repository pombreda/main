#ifndef BASIS_MATH_HPP_
#define BASIS_MATH_HPP_

#include <basis/configure.hpp>

namespace math
{
	template<typename Type>
	Type abs(const Type& value)
	{
		return value < Type() ? -value : value;
	}

	template<typename Type>
	ssize_t sign(const Type& value)
	{
		return value < Type() ? -1 : 1;
	}

	template<typename Type1, typename Type2>
	ssize_t sign(const Type1& a, const Type2& b)
	{
		return (a < Type1() && Type2() < b) || (Type1() < a && b < Type2()) ? -1 : 1;
	}

	ssize_t nod(ssize_t a, ssize_t b);

	struct fraction;

	template<unsigned long N>
	struct bin
	{
		enum {
			value = (N % 10) + 2 * bin<N / 10>::value
		};
	};

	template<>
	struct bin<0>
	{
		enum {
			value = 0
		};
	};
}

#include <basis/~math/fixed.hpp>
#include <basis/~math/fraction.hpp>
#include <basis/~math/impl_fixed.hpp>
#include <basis/~math/impl_fraction.hpp>

#endif
