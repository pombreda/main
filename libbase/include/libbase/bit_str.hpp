﻿#ifndef _LIBBASE_BIT_STR_HPP_
#define _LIBBASE_BIT_STR_HPP_

#include <libbase/std.hpp>
#include <libbase/bit.hpp>
#include <libbase/string.hpp>

namespace Base {

	template<typename Type>
	struct BitMask {
		static Type from_str(const ustring & in, size_t lim = 0)
		{
			// count bits from 1
			Type Result = 0;
			intmax_t bit = 0;
			ustring tmp(in);
			lim = Bits::Limit<Type>(lim);
			while (Base::String::Cut(tmp, bit)) {
				if (Bits::GOOD_BIT<Type>(--bit))
					Bits::set(Result, bit);
			}
			return Result;
		}

		static Type from_str_0(const ustring & in, size_t lim = 0)
		{
			// count bits from zero
			Type Result = 0;
			intmax_t bit = 0;
			ustring tmp(in);
			lim = Bits::Limit < Type > (lim);
			while (Base::String::Cut(tmp, bit)) {
				if (Bits::GOOD_BIT< Type > (bit))
					Bits::set(Result, bit);
			}
			return Result;
		}

		static ustring to_str(Type in, size_t lim = 0)
		{
			// count bits from 1
			ustring Result;
			lim = Bits::Limit < Type > (lim);
			for (size_t bit = 0; bit < lim; ++bit) {
				if (Bits::check(in, bit)) {
					Base::String::Inplace::add_word(Result, to_str(bit + 1), L",");
				}
			}
			return Result;
		}

		static ustring to_str_0(Type in, size_t lim = 0)
		{
			// count bits from zero
			ustring Result;
			lim = Bits::Limit < Type > (lim);
			for (size_t bit = 0; bit < lim; ++bit) {
				if (Bits::check(in, bit)) {
					Base::String::Inplace::add_word(Result, to_str(bit), L",");
				}
			}
			return Result;
		}

		static ustring to_str_bin(Type in, size_t lim = 0)
		{
			ustring Result;
			uintmax_t flag = (uintmax_t)1 << (Bits::Limit < Type > (lim) - 1);
			while (flag) {
				Result += Flags::check(in, (Type)flag) ? L'1' : L'0';
				flag >>= 1;
			}
			return Result;
		}

		static ustring to_str_num(Type in, size_t lim = 0)
		{
			ustring Result;
			uintmax_t flag = (uintmax_t)1 << (Bits::Limit < Type > (lim) - 1);
			while (flag) {
				if (Flags::check(in, (Type)flag)) {
					wchar_t buf[64];
					Cstr::convert_num(buf, flag);
					Base::String::Inplace::add_word(Result, buf, L",");
				}
				flag >>= 1;
			}
			return Result;
		}
	};

}

#endif
