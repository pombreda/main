#ifndef SARALIB_STL_ALGORITHM_HPP_
#define SARALIB_STL_ALGORITHM_HPP_

#include "impl_types.hpp"
#include "impl_iterator.hpp"
#include "impl_utility.hpp"

namespace sarastd {
	template<typename InputIt, typename UnaryPredicate>
	bool all_of(InputIt first, InputIt last, UnaryPredicate p)
	{
		for (; first != last; ++first)
			if (!p(*first))
				return false;
		return true;
	}

	template<typename InputIt, typename UnaryPredicate>
	bool any_of(InputIt first, InputIt last, UnaryPredicate p)
	{
		for (; first != last; ++first)
			if (p(*first))
				return true;
		return false;
	}

	template<typename InputIt, typename UnaryPredicate>
	bool none_of(InputIt first, InputIt last, UnaryPredicate p)
	{
		for (; first != last; ++first)
			if (p(*first))
				return false;
		return true;
	}

	template<typename InputIt, typename UnaryFunction>
	UnaryFunction for_each(InputIt first, InputIt last, UnaryFunction f)
	{
		for (; first != last; ++first)
			f(*first);
		return f;
	}

	template<typename InputIt, typename T>
	typename iterator_traits<InputIt>::difference_type count(InputIt first, InputIt last, const T& value)
	{
		typename iterator_traits<InputIt>::difference_type ret = 0;
		for (; first != last; ++first)
			if (*first == value)
				++ret;
		return ret;
	}

	template<typename InputIt, typename UnaryPredicate>
	typename iterator_traits<InputIt>::difference_type count_if(InputIt first, InputIt last, UnaryPredicate p)
	{
		typename iterator_traits<InputIt>::difference_type ret = 0;
		for (; first != last; ++first)
			if (p(*first))
				++ret;
		return ret;
	}

	template<typename InputIt, typename T>
	InputIt find(InputIt first, InputIt last, const T& value)
	{
		for (; first != last; ++first)
			if (*first == value)
				return first;
		return last;
	}

	template<typename InputIt, typename UnaryPredicate>
	InputIt find_if(InputIt first, InputIt last, UnaryPredicate p)
	{
		for (; first != last; ++first)
			if (p(*first))
				return first;
		return last;
	}

	template<typename InputIt, typename UnaryPredicate>
	InputIt find_if_not(InputIt first, InputIt last, UnaryPredicate q)
	{
		for (; first != last; ++first)
			if (!q(*first))
				return first;
		return last;
	}

	template<typename InputIt, typename ForwardIt>
	InputIt find_first_of(InputIt first, InputIt last, ForwardIt s_first, ForwardIt s_last)
	{
		for (; first != last; ++first)
			for (ForwardIt it = s_first; it != s_last; ++it)
				if (*first == *it)
					return first;
		return last;
	}

	template<typename InputIt, typename ForwardIt, typename BinaryPredicate>
	InputIt find_first_of(InputIt first, InputIt last, ForwardIt s_first, ForwardIt s_last, BinaryPredicate p)
	{
		for (; first != last; ++first)
			for (ForwardIt it = s_first; it != s_last; ++it)
				if (p(*first, *it))
					return first;
		return last;
	}

	template<typename ForwardIt>
	ForwardIt adjacent_find(ForwardIt first, ForwardIt last)
	{
		if (first == last)
			return last;
		ForwardIt next = first;
		++next;
		for (; next != last; ++next, ++first)
			if (*first == *next)
				return first;
		return last;
	}

	template<typename ForwardIt, typename BinaryPredicate>
	ForwardIt adjacent_find(ForwardIt first, ForwardIt last, BinaryPredicate p)
	{
		if (first == last)
			return last;
		ForwardIt next = first;
		++next;
		for (; next != last; ++next, ++first)
			if (p(*first, *next))
				return first;
		return last;
	}

	template<typename ForwardIt1, typename ForwardIt2>
	ForwardIt1 search(ForwardIt1 first, ForwardIt1 last, ForwardIt2 s_first, ForwardIt2 s_last)
	{
		for (;; ++first) {
			ForwardIt1 it = first;
			for (ForwardIt2 s_it = s_first;; ++it, ++s_it) {
				if (s_it == s_last)
					return first;
				if (it == last)
					return last;
				if (!(*it == *s_it))
					break;
			}
		}
	}

	template<typename ForwardIt1, typename ForwardIt2, typename BinaryPredicate>
	ForwardIt1 search(ForwardIt1 first, ForwardIt1 last, ForwardIt2 s_first, ForwardIt2 s_last, BinaryPredicate p)
	{
		for (;; ++first) {
			ForwardIt1 it = first;
			for (ForwardIt2 s_it = s_first;; ++it, ++s_it) {
				if (s_it == s_last)
					return first;
				if (it == last)
					return last;
				if (!p(*it, *s_it))
					break;
			}
		}
	}

	template<typename ForwardIt, typename Size, typename T>
	ForwardIt search_n(ForwardIt first, ForwardIt last, Size count, const T& value)
	{
		Size curr_count = 0;
		ForwardIt result, t_last = first;
		sarastd::advance(t_last, sarastd::distance(first, last) - count + 1);

		for (; first != t_last; ++first) {
			curr_count = 0;
			result = first;
			while (*first == value) {
				++curr_count;
				if (curr_count == count)
					return result;
				++first;
			}
		}
		return last;
	}

	template<typename ForwardIt, typename Size, typename T, typename BinaryPredicate>
	ForwardIt search_n(ForwardIt first, ForwardIt last, Size count, const T& value, BinaryPredicate p)
	{
		Size curr_count = 0;
		ForwardIt result, t_last = first;
		sarastd::advance(t_last, sarastd::distance(first, last) - count + 1);

		for (; first != t_last; ++first) {
			curr_count = 0;
			result = first;
			while (p(*first == value)) {
				++curr_count;
				if (curr_count == count)
					return result;
				++first;
			}
		}
		return last;
	}

	template<typename InputIt, typename OutputIt, typename UnaryOperation>
	OutputIt transform(InputIt first1, InputIt last1, OutputIt d_first, UnaryOperation unary_op)
	{
		while (first1 != last1) {
			*d_first = unary_op(*first1);
			++d_first;
			++first1;
		}
		return d_first;
	}

	template<typename InputIt1, typename InputIt2, typename OutputIt, typename BinaryOperation>
	OutputIt transform(InputIt1 first1, InputIt1 last1, InputIt2 first2, OutputIt d_first, BinaryOperation binary_op)
	{
		while (first1 != last1) {
			*d_first = binary_op(*first1, *first2);
			++d_first;
			++first1;
			++first2;
		}
		return d_first;
	}

	template<typename ForwardIt, typename Generator>
	void generate(ForwardIt first, ForwardIt last, Generator g)
	{
		while (first != last) {
			*first = g();
			++first;
		}
	}

	template<typename OutputIt, typename Size, typename Generator>
	OutputIt generate_n(OutputIt first, Size count, Generator g)
	{
		for (Size i = 0; i < count; ++i) {
			*first = g();
			++first;
		}
		return first;
	}

	template<typename ForwardIt, typename T>
	ForwardIt remove(ForwardIt first, ForwardIt last, const T& value)
	{
		ForwardIt result = first;
		for (; first != last; ++first) {
			if (!(*first == value)) {
				*result = *first;
				++result;
			}
		}
		return result;
	}

	template<typename ForwardIt, typename UnaryPredicate>
	ForwardIt remove_if(ForwardIt first, ForwardIt last, UnaryPredicate p)
	{
		ForwardIt result = first;
		for (; first != last; ++first) {
			if (!p(*first)) {
				*result = *first;
				++result;
			}
		}
		return result;
	}

	template<typename InputIt, typename OutputIt, typename T>
	OutputIt remove_copy(InputIt first, InputIt last, OutputIt d_first, const T& value)
	{
		for (; first != last; ++first)
			if (!(*first == value)) {
				*d_first = *first;
				++d_first;
			}
		return d_first;
	}

	template<typename InputIt, typename OutputIt, typename UnaryPredicate>
	OutputIt remove_copy_if(InputIt first, InputIt last, OutputIt d_first, UnaryPredicate p)
	{
		for (; first != last; ++first)
			if (!p(*first)) {
				*d_first = *first;
				++d_first;
			}
		return d_first;
	}

	template<typename ForwardIt, typename T>
	void replace(ForwardIt first, ForwardIt last, const T& old_value, const T& new_value)
	{
		for (; first != last; ++first)
			if (*first == old_value)
				*first = new_value;
	}

	template<typename ForwardIt, typename UnaryPredicate, typename T>
	void replace_if(ForwardIt first, ForwardIt last, UnaryPredicate p, const T& new_value)
	{
		for (; first != last; ++first)
			if (p(*first))
				*first = new_value;
	}

	template<typename InputIt, typename OutputIt, typename T>
	OutputIt replace_copy(InputIt first, InputIt last, OutputIt d_first, const T& old_value, const T& new_value)
	{
		for (; first != last; ++first) {
			*d_first = (*first == old_value) ? new_value : *first;
			++d_first;
		}
		return d_first;
	}

	template<typename InputIt, typename OutputIt, typename UnaryPredicate, typename T>
	OutputIt replace_copy_if(InputIt first, InputIt last, OutputIt d_first, UnaryPredicate p, const T& new_value)
	{
		for (; first != last; ++first) {
			*d_first = p(*first) ? new_value : *first;
			++d_first;
		}
		return d_first;
	}

	template<typename ForwardIt1, typename ForwardIt2>
	ForwardIt1 swap_ranges(ForwardIt1 first1, ForwardIt1 last1, ForwardIt2 first2)
	{
		while (first1 != last1) {
			sarastd::iter_swap(first1, first2);
			++first1;
			++first2;
		}
		return first2;
	}

	template<typename BidirIt>
	void reverse(BidirIt first, BidirIt last)
	{
		while ((first != last) && (first != --last)) {
			sarastd::swap(*first, *last);
			++first;
		}
	}

	template<typename BidirIt, typename OutputIt>
	OutputIt reverse_copy(BidirIt first, BidirIt last, OutputIt d_first)
	{
		while (first != last) {
			*(d_first) = *(--last);
			++d_first;
		}
		return d_first;
	}

	template<typename ForwardIt>
	void rotate(ForwardIt first, ForwardIt n_first, ForwardIt last)
	{
		ForwardIt next = n_first;
		while (first != next) {
			sarastd::swap(*first, *next);
			++first;
			++next;
			if (next == last)
				next = n_first;
			else if (first == n_first)
				n_first = next;
		}
	}

	template<typename ForwardIt, typename OutputIt>
	OutputIt rotate_copy(ForwardIt first, ForwardIt n_first, ForwardIt last, OutputIt d_first)
	{
		d_first = sarastd::copy(n_first, last, d_first);
		return sarastd::copy(first, n_first, d_first);
	}

	template<typename RandomIt, typename RandomFunc>
	void random_shuffle(RandomIt first, RandomIt last, RandomFunc& r)
	{
		typename sarastd::iterator_traits<RandomIt>::difference_type i, n;
		n = last - first;
		for (i = n - 1; i > 0; --i) {
			using sarastd::swap;
			swap(first[i], first[r(i + 1)]);
		}
	}

	template<typename ForwardIt>
	ForwardIt unique(ForwardIt first, ForwardIt last)
	{
		if (first == last)
			return last;

		ForwardIt result = first;
		while (++first != last) {
			if (!(*result == *first)) {
				*(++result) = *first;
			}
		}
		return ++result;
	}

	template<typename ForwardIt, typename BinaryPredicate>
	ForwardIt unique(ForwardIt first, ForwardIt last, BinaryPredicate p)
	{
		if (first == last)
			return last;

		ForwardIt result = first;
		while (++first != last) {
			if (!p(*result, *first)) {
				*(++result) = *first;
			}
		}
		return ++result;
	}

	template<typename ForwardIt, typename OutputIt>
	ForwardIt unique_copy(ForwardIt first, ForwardIt last, OutputIt d_first)
	{
		if (first == last)
			return d_first;

		*d_first = *first;
		while (++first != last) {
			if (!(*d_first == *first)) {
				*(++d_first) = *first;
			}
		}
		return ++d_first;
	}

	template<typename ForwardIt, typename OutputIt, typename BinaryPredicate>
	ForwardIt unique_copy(ForwardIt first, ForwardIt last, OutputIt d_first, BinaryPredicate p)
	{
		if (first == last)
			return d_first;

		*d_first = *first;
		while (++first != last) {
			if (!p(*d_first, *first)) {
				*(++d_first) = *first;
			}
		}
		return ++d_first;
	}

	template<typename ForwardIt, typename T>
	ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value)
	{
		ForwardIt it;
		typename sarastd::iterator_traits<ForwardIt>::difference_type count, step;
		count = sarastd::distance(first, last);

		while (count > 0) {
			it = first;
			step = count / 2;
			sarastd::advance(it, step);
			if (*it < value) {
				first = ++it;
				count -= step + 1;
			} else
				count = step;
		}
		return first;
	}

	template<typename ForwardIt, typename T, typename Compare>
	ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp)
	{
		ForwardIt it;
		typename sarastd::iterator_traits<ForwardIt>::difference_type count, step;
		count = sarastd::distance(first, last);

		while (count > 0) {
			it = first;
			step = count / 2;
			sarastd::advance(it, step);
			if (comp(*it, value)) {
				first = ++it;
				count -= step + 1;
			} else
				count = step;
		}
		return first;
	}

	template<typename ForwardIt, typename T>
	ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value)
	{
		ForwardIt it;
		typename sarastd::iterator_traits<ForwardIt>::distance_type count, step;
		count = sarastd::distance(first, last);

		while (count > 0) {
			it = first;
			step = count / 2;
			sarastd::advance(it, step);
			if (!(value < *it)) {
				first = ++it;
				count -= step + 1;
			} else
				count = step;
		}
		return first;
	}

	template<typename ForwardIt, typename T, typename Compare>
	ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp)
	{
		ForwardIt it;
		typename sarastd::iterator_traits<ForwardIt>::distance_type count, step;
		count = sarastd::distance(first, last);

		while (count > 0) {
			it = first;
			step = count / 2;
			sarastd::advance(it, step);
			if (!comp(value, *it)) {
				first = ++it;
				count -= step + 1;
			} else
				count = step;
		}
		return first;
	}

	template<typename ForwardIt, typename T>
	bool binary_search(ForwardIt first, ForwardIt last, const T& value)
	{
		first = sarastd::lower_bound(first, last, value);
		return (!(first == last) && !(value < *first));
	}

	template<typename ForwardIt, typename T, typename Compare>
	bool binary_search(ForwardIt first, ForwardIt last, const T& value, Compare comp)
	{
		first = sarastd::lower_bound(first, last, value, comp);
		return (!(first == last) && !comp(value, *first));
	}

	template<typename ForwardIt, typename T>
	sarastd::pair<ForwardIt, ForwardIt> equal_range(ForwardIt first, ForwardIt last, const T& value)
	{
		return sarastd::make_pair(sarastd::lower_bound(first, last, value), sarastd::upper_bound(first, last, value));
	}

	template<typename ForwardIt, typename T, typename Compare>
	sarastd::pair<ForwardIt, ForwardIt> equal_range(ForwardIt first, ForwardIt last, const T& value, Compare comp)
	{
		return sarastd::make_pair(sarastd::lower_bound(first, last, value, comp), sarastd::upper_bound(first, last, value, comp));
	}

	template<typename InputIt1, typename InputIt2, typename OutputIt>
	OutputIt merge(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, OutputIt d_first)
	{
		for (; first1 != last1; ++d_first) {
			if (first2 == last2) {
				return sarastd::copy(first1, last1, d_first);
			}
			if (*first2 < *first1) {
				*d_first = *first2;
				++first2;
			} else {
				*d_first = *first1;
				++first1;
			}
		}
		return sarastd::copy(first2, last2, d_first);
	}

	template<typename InputIt1, typename InputIt2, typename OutputIt, typename Compare>
	OutputIt merge(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, OutputIt d_first, Compare comp)
	{
		for (; first1 != last1; ++d_first) {
			if (first2 == last2) {
				return sarastd::copy(first1, last1, d_first);
			}
			if (comp(*first2, *first1)) {
				*d_first = *first2;
				++first2;
			} else {
				*d_first = *first1;
				++first1;
			}
		}
		return sarastd::copy(first2, last2, d_first);
	}

	template<typename InputIt1, typename InputIt2>
	bool includes(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
	{
		for (; first2 != last2; ++first1) {
			if (first1 == last1 || *first2 < *first1)
				return false;
			if (!(*first1 < *first2))
				++first2;
		}
		return true;
	}

	template<typename InputIt1, typename InputIt2, typename Compare>
	bool includes(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, Compare comp)
	{
		for (; first2 != last2; ++first1) {
			if (first1 == last1 || comp(*first2, *first1))
				return false;
			if (!comp(*first1, *first2))
				++first2;
		}
		return true;
	}

	template<typename InputIt1, typename InputIt2, typename OutputIt>
	OutputIt set_difference(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, OutputIt d_first)
	{
		while (first1 != last1) {
			if (first2 == last2)
				return sarastd::copy(first1, last1, d_first);

			if (*first1 < *first2) {
				*d_first = *first1;
				++d_first;
				++first1;
			} else {
				if (!(*first2 < *first1)) {
					++first1;
				}
				++first2;
			}
		}
		return d_first;
	}

	template<typename InputIt1, typename InputIt2, typename OutputIt, typename Compare>
	OutputIt set_difference(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, OutputIt d_first, Compare comp)
	{
		while (first1 != last1) {
			if (first2 == last2)
				return sarastd::copy(first1, last1, d_first);

			if (comp(*first1, *first2)) {
				*d_first = *first1;
				++d_first;
				++first1;
			} else {
				if (!comp(*first2, *first1)) {
					++first1;
				}
				++first2;
			}
		}
		return d_first;
	}

	template<typename InputIt1, typename InputIt2, typename OutputIt>
	OutputIt set_intersection(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, OutputIt d_first)
	{
		while (first1 != last1 && first2 != last2) {
			if (*first1 < *first2) {
				++first1;
			} else {
				if (!(*first2 < *first1)) {
					*d_first = *first1;
					++d_first;
					++first1;
				}
				++first2;
			}
		}
		return d_first;
	}

	template<typename InputIt1, typename InputIt2, typename OutputIt, typename Compare>
	OutputIt set_intersection(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, OutputIt d_first, Compare comp)
	{
		while (first1 != last1 && first2 != last2) {
			if (comp(*first1, *first2)) {
				++first1;
			} else {
				if (!comp(*first2, *first1)) {
					*d_first = *first1;
					++d_first;
					++first1;
				}
				++first2;
			}
		}
		return d_first;
	}

	template<typename InputIt1, typename InputIt2, typename OutputIt>
	OutputIt set_symmetric_difference(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, OutputIt d_first)
	{
		while (first1 != last1) {
			if (first2 == last2)
				return sarastd::copy(first1, last1, d_first);

			if (*first1 < *first2) {
				*d_first = *first1;
				++d_first;
				++first1;
			} else {
				if (*first2 < *first1) {
					*d_first = *first2;
					++d_first;
				} else {
					++first1;
				}
				++first2;
			}
		}
		return sarastd::copy(first2, last2, d_first);
	}

	template<typename InputIt1, typename InputIt2, typename OutputIt, typename Compare>
	OutputIt set_symmetric_difference(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, OutputIt d_first, Compare comp)
	{
		while (first1 != last1) {
			if (first2 == last2)
				return sarastd::copy(first1, last1, d_first);

			if (comp(*first1, *first2)) {
				*d_first = *first1;
				++d_first;
				++first1;
			} else {
				if (comp(*first2, *first1)) {
					*d_first = *first2;
					++d_first;
				} else {
					++first1;
				}
				++first2;
			}
		}
		return sarastd::copy(first2, last2, d_first);
	}

	template<typename InputIt1, typename InputIt2, typename OutputIt>
	OutputIt set_union(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, OutputIt d_first)
	{
		for (; first1 != last1; ++d_first) {
			if (first2 == last2)
				return sarastd::copy(first1, last1, d_first);
			if (*first2 < *first1) {
				*d_first = *first2;
				++first2;
			} else {
				*d_first = *first1;
				if (!(*first1 < *first2))
					++first2;
				++first1;
			}
		}
		return sarastd::copy(first2, last2, d_first);
	}

	template<typename InputIt1, typename InputIt2, typename OutputIt, typename Compare>
	OutputIt set_union(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, OutputIt d_first, Compare comp)
	{
		for (; first1 != last1; ++d_first) {
			if (first2 == last2)
				return sarastd::copy(first1, last1, d_first);
			if (comp(*first2, *first1)) {
				*d_first = *first2;
				++first2;
			} else {
				*d_first = *first1;
				if (!comp(*first1, *first2))
					++first2;
				++first1;
			}
		}
		return sarastd::copy(first2, last2, d_first);
	}

	template<typename ForwardIt>
	ForwardIt max_element(ForwardIt first, ForwardIt last)
	{
		if (first == last) {
			return last;
		}
		ForwardIt largest = first;
		++first;
		for (; first != last; ++first) {
			if (*largest < *first) {
				largest = first;
			}
		}
		return largest;
	}

	template<typename ForwardIt, typename Compare>
	ForwardIt max_element(ForwardIt first, ForwardIt last, Compare comp)
	{
		if (first == last) {
			return last;
		}
		ForwardIt largest = first;
		++first;
		for (; first != last; ++first) {
			if (comp(*largest, *first)) {
				largest = first;
			}
		}
		return largest;
	}

	template<typename ForwardIt>
	ForwardIt min_element(ForwardIt first, ForwardIt last)
	{
		if (first == last)
			return last;

		ForwardIt smallest = first;
		++first;
		for (; first != last; ++first) {
			if (*first < *smallest) {
				smallest = first;
			}
		}
		return smallest;
	}

	template<typename ForwardIt, typename Compare>
	ForwardIt min_element(ForwardIt first, ForwardIt last, Compare comp)
	{
		if (first == last)
			return last;

		ForwardIt smallest = first;
		++first;
		for (; first != last; ++first) {
			if (comp(*first, *smallest)) {
				smallest = first;
			}
		}
		return smallest;
	}

	template<typename T>
	sarastd::pair<const T&, const T&> minmax(const T& a, const T& b)
	{
		return (b < a) ? sarastd::make_pair(b, a) : sarastd::make_pair(a, b);
	}

	template<typename T, typename Compare>
	sarastd::pair<const T&, const T&> minmax(const T& a, const T& b, Compare comp)
	{
		return comp(b, a) ? sarastd::make_pair(b, a) : sarastd::make_pair(a, b);
	}

	template<typename BidirIt>
	bool next_permutation(BidirIt first, BidirIt last)
	{
		if (first == last)
			return false;
		BidirIt i = last;
		if (first == --i)
			return false;

		while (1) {
			BidirIt i1, i2;

			i1 = i;
			if (*--i < *i1) {
				i2 = last;
				while (!(*i < *--i2))
					;
				sarastd::iter_swap(i, i2);
				sarastd::reverse(i1, last);
				return true;
			}
			if (i == first) {
				sarastd::reverse(first, last);
				return false;
			}
		}
	}

	template<typename BidirIt>
	bool prev_permutation(BidirIt first, BidirIt last)
	{
		if (first == last)
			return false;
		BidirIt i = last;
		if (first == --i)
			return false;

		while (1) {
			BidirIt i1, i2;

			i1 = i;
			if (*i1 < *--i) {
				i2 = last;
				while (!(*--i2 < *i))
					;
				sarastd::iter_swap(i, i2);
				sarastd::reverse(i1, last);
				return true;
			}
			if (i == first) {
				sarastd::reverse(first, last);
				return false;
			}
		}
	}
}

#endif
