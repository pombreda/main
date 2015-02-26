#ifndef BASIS_CONFIGURE_OBLIGATORY_TYPES_HPP_
#define BASIS_CONFIGURE_OBLIGATORY_TYPES_HPP_

#include <stdint.h>

typedef const void* PCVOID;

template<typename Type, size_t N>
constexpr size_t lengthof(Type (&)[N])
{
	return N;
}

#endif
