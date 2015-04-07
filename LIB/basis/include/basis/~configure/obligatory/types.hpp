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

	atomic_int& operator ++() noexcept {InterlockedExchangeAdd(&value, +1); return *this;}
	atomic_int& operator --() noexcept {InterlockedExchangeAdd(&value, -1); return *this;}
	atomic_int operator ++(int) noexcept {value_type ret = InterlockedExchangeAdd(&value, +1); return atomic_int(ret);}
	atomic_int operator --(int) noexcept {value_type ret = InterlockedExchangeAdd(&value, -1); return atomic_int(ret);}
	operator ssize_t() const {return value;}
	bool add_if_not_equal(ssize_t addition, ssize_t compare) volatile noexcept
	{
		value_type current;
		do
		{
			current = value;
			if (current == compare)
				return false;
		} while (InterlockedCompareExchange(&value, current + addition, current) != current);
		return true;
	}

	bool operator ==(int compare) const noexcept {return value == compare;}

private:
	value_type value;
};

#endif
