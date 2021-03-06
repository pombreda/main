#ifndef BASIS_MATH_HPP_
#define BASIS_MATH_HPP_

#include <basis/configure.hpp>

namespace math
{
	template<typename Type>
	Type abs(const Type& value)
	{
		return value < Type(0) ? -value : value;
	}

	template<typename Type>
	ssize_t sign(const Type& value)
	{
		return value < Type(0) ? -1 : 1;
	}

	ssize_t nod(ssize_t left, ssize_t right);

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
