#ifndef BASIS_CONFIGURE_OBLIGATORY_TYPES_HPP_
#define BASIS_CONFIGURE_OBLIGATORY_TYPES_HPP_

#include <stdint.h>

typedef const void* PCVOID;

template<typename Type, size_t N>
constexpr size_t lengthof(Type (&)[N])
{
	return N;
}

class atomic_int
{
	using value_type = long int;
public:
	atomic_int() noexcept : value() {}
	atomic_int(value_type value) noexcept : value(value) {}

	atomic_int& operator ++() noexcept {InterlockedAdd(&value, +1); return *this;}
	atomic_int& operator --() noexcept {InterlockedAdd(&value, -1); return *this;}
	operator ssize_t() const {return value;}

private:
	value_type value;
};

#endif
