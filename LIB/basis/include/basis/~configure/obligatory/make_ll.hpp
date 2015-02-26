#ifndef BASIS_CONFIGURE_OBLIGATORY_MAKE_LL_HPP_
#define BASIS_CONFIGURE_OBLIGATORY_MAKE_LL_HPP_

inline uint64_t make_uint64(uint32_t high, uint32_t low)
{
	return uint64_t(high) << 32 | low;
}

inline uint32_t high_part_64(uint64_t arg64)
{
	return uint32_t(arg64 >> 32);
}

inline uint32_t low_part_64(uint64_t arg64)
{
	return uint32_t(arg64 & 0xFFFFFFFFULL);
}

inline uint32_t reverse_bits(uint32_t val)
{
	val = ((val >> 16) & 0x0000ffffu) | ((val & 0x0000ffffu) << 16);
	val = ((val >>  8) & 0x00ff00ffu) | ((val & 0x00ff00ffu) <<  8);
	val = ((val >>  4) & 0x0f0f0f0fu) | ((val & 0x0f0f0f0fu) <<  4);
	val = ((val >>  2) & 0x33333333u) | ((val & 0x33333333u) <<  2);
	val = ((val >>  1) & 0x55555555u) | ((val & 0x55555555u) <<  1);
	return val;
}

#endif
