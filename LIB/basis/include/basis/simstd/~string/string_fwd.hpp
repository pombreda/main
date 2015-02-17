#ifndef BASIS_STRING_STRING_FWD_HPP_
#define BASIS_STRING_STRING_FWD_HPP_

namespace simstd {
	template<typename CharType>
	struct char_traits;

	template<typename CharType, typename Traits, typename Allocator>
	class basic_string;

	typedef simstd::basic_string<char, simstd::char_traits<char>, simstd::allocator<char>>          string;
	typedef simstd::basic_string<wchar_t, simstd::char_traits<wchar_t>, simstd::allocator<wchar_t>> wstring;

}

typedef simstd::basic_string<char, simstd::char_traits<char>, simstd::allocator<char>>          astring;
typedef simstd::basic_string<wchar_t, simstd::char_traits<wchar_t>, simstd::allocator<wchar_t>> ustring;

#endif
