#ifndef BASIS_CONFIGURE_OBLIGATORY_TYPES_HPP_
#define BASIS_CONFIGURE_OBLIGATORY_TYPES_HPP_

#include <stdint.h>

typedef const void* PCVOID;

inline uint32_t reverse_bits(uint32_t val)
{
	val = ((val >> 16) & 0x0000ffffu) | ((val & 0x0000ffffu) << 16);
	val = ((val >>  8) & 0x00ff00ffu) | ((val & 0x00ff00ffu) <<  8);
	val = ((val >>  4) & 0x0f0f0f0fu) | ((val & 0x0f0f0f0fu) <<  4);
	val = ((val >>  2) & 0x33333333u) | ((val & 0x33333333u) <<  2);
	val = ((val >>  1) & 0x55555555u) | ((val & 0x55555555u) <<  1);
	return val;
}

template<unsigned long N>
struct bin {
	enum {
		value = (N % 10) + 2 * bin<N / 10>::value
	};
};

template<>
struct bin<0> {
	enum {
		value = 0
	};
};

#endif
