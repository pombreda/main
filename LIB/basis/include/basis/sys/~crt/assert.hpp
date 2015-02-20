#ifndef BASIS_SYS_CRT_ASSERT_HPP_
#define BASIS_SYS_CRT_ASSERT_HPP_

namespace crt {

	void assrt(const char* message, const char* file, int line);

	void assrt(const wchar_t* message, const char* file, int line);

}

#endif
