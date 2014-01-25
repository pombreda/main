﻿#ifndef LIBSTL_MEMORY_FUNCTIONAL_HPP_
#define LIBSTL_MEMORY_FUNCTIONAL_HPP_

namespace simstd {

	template<typename InputIt, typename ForwardIt>
	ForwardIt uninitialized_copy(InputIt first, InputIt last, ForwardIt d_first)
	{
		for (; first != last; ++first, ++d_first) {
			simstd::pvt::_construct(simstd::pvt::_addressof(*d_first), *first);
		}
		return d_first;
	}

	template<typename InputIt, typename Size, typename ForwardIt>
	ForwardIt uninitialized_copy_n(InputIt first, Size cnt, ForwardIt d_first)
	{
		for (; cnt > 0; ++first, ++d_first, --cnt) {
			simstd::pvt::_construct(simstd::pvt::_addressof(*d_first), *first);
		}
		return d_first;
	}

	template<typename ForwardIt, typename T>
	void uninitialized_fill(ForwardIt first, ForwardIt last, const T& val)
	{
		for (; first != last; ++first) {
			simstd::pvt::_construct(simstd::pvt::_addressof(*first), val);
		}
	}

	template<typename ForwardIt, typename Size, typename T>
	void uninitialized_fill_n(ForwardIt first, Size cnt, const T& val)
	{
		for (; cnt > 0; ++first, --cnt) {
			simstd::pvt::_construct(simstd::pvt::_addressof(*first), val);
		}
	}

	template<typename InputIt, typename ForwardIt>
	ForwardIt uninitialized_move(InputIt first, InputIt last, ForwardIt d_first)
	{
		for (; first != last; ++first, ++d_first) {
			simstd::pvt::_construct(simstd::addressof(*d_first), simstd::move(*first));
		}
		return d_first;
	}

	template<typename InputIt, typename Size, typename ForwardIt>
	ForwardIt uninitialized_move_n(InputIt first, Size cnt, ForwardIt d_first)
	{
		for (; cnt > 0; ++first, ++d_first, --cnt) {
			simstd::pvt::_construct(simstd::addressof(*d_first), simstd::move(*first));
		}
		return d_first;
	}

}

#endif
