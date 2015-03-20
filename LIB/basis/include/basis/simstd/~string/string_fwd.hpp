#ifndef BASIS_SIMSTD_STRING_STRING_FWD_HPP_
#define BASIS_SIMSTD_STRING_STRING_FWD_HPP_

namespace simstd {
	template<typename CharType>
	struct char_traits;

	template<typename CharType, typename Traits = simstd::char_traits<CharType>, typename Allocator = simstd::allocator<CharType>>
	class basic_string;

	using string = basic_string<char>;
	using wstring = basic_string<wchar_t>;
}

using astring = simstd::string;
using ustring = simstd::wstring;

#endif
