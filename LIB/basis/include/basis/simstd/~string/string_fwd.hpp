#ifndef BASIS_SIMSTD_STRING_STRING_FWD_HPP_
#define BASIS_SIMSTD_STRING_STRING_FWD_HPP_

namespace simstd {
	template<typename CharType>
	struct char_traits;

	template<typename CharType, typename Traits = simstd::char_traits<CharType>, typename Allocator = simstd::allocator<CharType>>
	class basic_string;

	using string = basic_string<char, char_traits<char>, allocator<char>>;
	using wstring = basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t>>;
}

using astring = simstd::basic_string<char, simstd::char_traits<char>, simstd::allocator<char>>;
using ustring = simstd::basic_string<wchar_t, simstd::char_traits<wchar_t>, simstd::allocator<wchar_t>>;

#endif
