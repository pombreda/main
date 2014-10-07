﻿#ifndef BASIS_STRING_BASIC_STRING2_HPP_
#define BASIS_STRING_BASIC_STRING2_HPP_

#include <basis/ext/pattern.hpp>
#include <basis/simstd/string>
#include <basis/simstd/iterator>
#include <basis/simstd/~memory/allocator.hpp>

//#include <basis/simstd/algorithm>
//#include <basis/simstd/algorithm>
//#include <basis/simstd/memory>
//#include <basis/sys/crt.hpp>

namespace simstd {

	namespace Test {
		ssize_t a_string(aPrintFunc printFunc);
		ssize_t w_string(wPrintFunc printFunc);
	}

}

namespace simstd {

	template<typename CharType,
		typename Traits = simstd::char_traits<CharType>,
		typename Allocator = simstd::allocator<CharType> >
	class basic_string2: private pvt::StrBase<CharType, Allocator> {
		typedef basic_string2                                this_type;
		typedef pvt::StrBase<CharType, Allocator>          base_type;

	public:
		typedef Traits                                      traits_type;
		typedef size_t                                      size_type;
		typedef ptrdiff_t                                   difference_type;

		typedef typename traits_type::char_type             value_type;
		typedef Allocator                                   allocator_type;
		typedef value_type &                                reference;
		typedef const value_type &                          const_reference;
		typedef value_type *                                pointer;
		typedef const value_type *                          const_pointer;

		typedef simstd::pvt::normal_iterator<pointer>       iterator;
		typedef simstd::pvt::normal_iterator<const_pointer> const_iterator;
		typedef simstd::reverse_iterator<iterator>          reverse_iterator;
		typedef simstd::reverse_iterator<const_iterator>    const_reverse_iterator;

		static const size_type npos = ~static_cast<size_type>(0);

		~basic_string2() = default;

		basic_string2();
		explicit basic_string2(const allocator_type& alloc);

		basic_string2(size_type count, value_type ch, const allocator_type& alloc = allocator_type());
		basic_string2(const this_type& other, size_type pos, size_type count = npos, const allocator_type& alloc = allocator_type());
		basic_string2(const_pointer str, size_type count, const allocator_type& alloc = allocator_type());
		basic_string2(const_pointer str, const allocator_type& alloc = allocator_type());
		template<typename Iterator>
		basic_string2(Iterator first, Iterator last, const allocator_type& alloc = allocator_type());
		basic_string2(const this_type& other);
		basic_string2(const this_type& other, const allocator_type& alloc);
		basic_string2(this_type&& other);
		basic_string2(this_type&& other, const allocator_type& alloc);
//		basic_string2(std::initializer_list<value_type> ilist, const allocator_type& alloc = allocator_type());

		this_type & operator =(const this_type & right);
		this_type & operator =(this_type && right);
		this_type & operator =(const_pointer right);
		this_type & operator =(value_type ch);

		bool empty() const;

		size_type capacity() const;
		size_type size() const;
		size_type length() const;

		const_pointer c_str() const;

		void swap(this_type & other);

		const_iterator cbegin() const;
		const_iterator begin() const;
		iterator begin();

		const_iterator cend() const;
		const_iterator end() const;
		iterator end();

		void clear();

		void reserve(size_type capa);

		int compare(const_pointer str) const;
		int compare(const this_type & str) const;
		int compare(size_type pos, size_type count, const this_type & str) const;
		int compare(size_type pos, size_type count, const_pointer str) const;
		int compare(size_type pos1, size_type count1, const_pointer str, size_type count2) const;

		this_type & append(size_type count, value_type ch);
		this_type & append(const this_type & str);
		this_type & append(const this_type & str, size_type pos, size_type count = npos);
		this_type & append(const_pointer str, size_type count);
		this_type & append(const_pointer str);
		template<typename Iterator>
		this_type & append(Iterator first, Iterator last);

		this_type & assign(size_type count, value_type ch);
		this_type & assign(const this_type & other);
		this_type & assign(const this_type & other, size_type pos, size_type count = npos);
		this_type & assign(this_type && other);
		this_type & assign(const_pointer str, size_type count);
		this_type & assign(const_pointer str);
		template<typename Iterator>
		this_type & assign(Iterator first, Iterator last);

		iterator insert(const_iterator pos, value_type ch);
		iterator insert(iterator pos, size_type count, value_type ch);
		this_type & insert(size_type index, size_type count, value_type ch);
		this_type & insert(size_type index, const_pointer str);
		this_type & insert(size_type index, const_pointer str, size_type count);
		this_type & insert(size_type index, const this_type & str);
		this_type & insert(size_type index, const this_type & str, size_type index_str, size_type count);

		this_type & operator +=(const this_type & str);
		this_type & operator +=(const_pointer str);
		this_type & operator +=(value_type ch);

		const_reference operator [](size_type in) const;
		reference operator [](size_type in);

		const_reference at(size_type in) const;
		reference at(size_type in);

		this_type & replace(size_type pos, size_type len, const this_type & other);
		this_type & replace(const_iterator first, const_iterator last, const this_type & other);
		this_type & replace(size_type pos, size_type len, const this_type & str, size_type subpos, size_type sublen = npos);
		template<typename Iterator>
		this_type & replace(const_iterator first, const_iterator last, Iterator first2, Iterator last2);
		this_type & replace(size_type pos, size_type len, const_pointer str, size_type count);
		this_type & replace(const_iterator first, const_iterator last, const_pointer str, size_type count);
		this_type & replace(size_type pos, size_type len, const_pointer str);
		this_type & replace(const_iterator first, const_iterator last, const_pointer str);
		this_type & replace(size_type pos, size_type len, size_type count, value_type ch);
		this_type & replace(const_iterator first, const_iterator last, size_type count, value_type ch);

		this_type substr(size_type pos = 0, size_type n = npos) const;

		this_type & erase(size_type pos = 0, size_type count = npos);
		iterator erase(const_iterator position);
		iterator erase(const_iterator first, const_iterator last);

		size_type find(const this_type & str, size_type pos = 0) const;
		size_type find(const_pointer str, size_type pos = 0) const;
		size_type find(value_type ch, size_type pos = 0) const;

		size_type rfind(const this_type & str, size_type pos = npos) const;
		size_type rfind(const_pointer str, size_type pos, size_type count) const;
		size_type rfind(const_pointer str, size_type pos = npos) const;
		size_type rfind(value_type ch, size_type pos = npos) const;

		size_type find_first_of(const this_type & str, size_type pos = 0) const;
		size_type find_last_of(const this_type & str, size_type pos = npos) const;
		size_type find_first_not_of(const this_type & str, size_type pos = 0) const;
		size_type find_last_not_of(const this_type & str, size_type pos = npos) const;

	private:
		basic_string2(const Allocator & alloc, const_pointer str, size_type count, size_type capacity);

		template<typename InputIt>
		basic_string2(InputIt first, InputIt last, const allocator_type& alloc, simstd::input_iterator_tag);
		template<typename ForwardIt>
		basic_string2(ForwardIt first, ForwardIt last, const allocator_type& alloc, simstd::forward_iterator_tag);

		void raw_append(size_type count, value_type ch);
		void raw_append(const_pointer str, size_type count);
		template<typename InputIt>
		void raw_append(InputIt first, InputIt last);

		template<typename InputIt>
		this_type & _append(InputIt first, InputIt last, simstd::input_iterator_tag);
		template<typename ForwardIt>
		this_type & _append(ForwardIt first, ForwardIt last, simstd::forward_iterator_tag);

		template<typename InputIt>
		this_type & _assign(InputIt first, InputIt last, simstd::input_iterator_tag);
		template<typename ForwardIt>
		this_type & _assign(ForwardIt first, ForwardIt last, simstd::forward_iterator_tag);

		pointer _str() const;

		size_type get_necessary_capacity(ptrdiff_t addToSize) const;

		void split_and_clear(size_type minCapacity);

		void split_and_copy(size_type minCapacity);

		static int compare(const_pointer str1, size_type count1, const_pointer str2, size_type count2);

		size_type find(const_pointer str, size_type pos, size_type length) const;

		bool is_same_str(const_pointer str) const;

		static const size_t MIN_ALLOC_BLOCK = 16;

		template<typename C, typename T, typename A> friend basic_string2<C, T, A> operator +(const basic_string2<C, T, A> & left, const basic_string2<C, T, A> & right);
		template<typename C, typename T, typename A> friend basic_string2<C, T, A> operator +(const C * left, const basic_string2<C, T, A> & right);
		template<typename C, typename T, typename A> friend basic_string2<C, T, A> operator +(C left, const basic_string2<C, T, A> & right);
		template<typename C, typename T, typename A> friend basic_string2<C, T, A> operator +(const basic_string2<C, T, A> & left, const C * right);
		template<typename C, typename T, typename A> friend basic_string2<C, T, A> operator +(const basic_string2<C, T, A> & left, C right);

		template<typename C, typename T, typename A> friend bool operator ==(const basic_string2<C, T, A> & left, const basic_string2<C, T, A> & right);
		template<typename C, typename T, typename A> friend bool operator ==(const C * left, const basic_string2<C, T, A> & right);
		template<typename C, typename T, typename A> friend bool operator ==(const basic_string2<C, T, A> & left, const C * right);
		template<typename C, typename T, typename A> friend bool operator !=(const basic_string2<C, T, A> & left, const basic_string2<C, T, A> & right);
		template<typename C, typename T, typename A> friend bool operator !=(const C * left, const basic_string2<C, T, A> & right);
		template<typename C, typename T, typename A> friend bool operator !=(const basic_string2<C, T, A> & left, const C * right);
		template<typename C, typename T, typename A> friend bool operator <(const basic_string2<C, T, A> & left, const basic_string2<C, T, A> & right);
		template<typename C, typename T, typename A> friend bool operator <(const C * left, const basic_string2<C, T, A> & right);
		template<typename C, typename T, typename A> friend bool operator <(const basic_string2<C, T, A> & left, const C * right);
		template<typename C, typename T, typename A> friend bool operator <=(const basic_string2<C, T, A> & left, const basic_string2<C, T, A> & right);
		template<typename C, typename T, typename A> friend bool operator <=(const C * left, const basic_string2<C, T, A> & right);
		template<typename C, typename T, typename A> friend bool operator <=(const basic_string2<C, T, A> & left, const C * right);
		template<typename C, typename T, typename A> friend bool operator >(const basic_string2<C, T, A> & left, const basic_string2<C, T, A> & right);
		template<typename C, typename T, typename A> friend bool operator >(const C * left, const basic_string2<C, T, A> & right);
		template<typename C, typename T, typename A> friend bool operator >(const basic_string2<C, T, A> & left, const C * right);
		template<typename C, typename T, typename A> friend bool operator >=(const basic_string2<C, T, A> & left, const basic_string2<C, T, A> & right);
		template<typename C, typename T, typename A> friend bool operator >=(const C * left, const basic_string2<C, T, A> & right);
		template<typename C, typename T, typename A> friend bool operator >=(const basic_string2<C, T, A> & left, const C * right);
	};

	template<typename C, typename T, typename A>
	basic_string2<C, T, A>::basic_string2() :
		base_type(A())
	{
	}

	template<typename C, typename T, typename A>
	basic_string2<C, T, A>::basic_string2(const A& alloc):
		base_type(alloc)
	{
	}

	template<typename C, typename T, typename A>
	basic_string2<C, T, A>::basic_string2(size_type count, value_type ch, const allocator_type& alloc):
		base_type(alloc, count)
	{
		raw_append(count, ch);
	}

	template<typename C, typename T, typename A>
	basic_string2<C, T, A>::basic_string2(const this_type& other, size_type pos, size_type count, const allocator_type& alloc):
		base_type(alloc, (count == npos) ? count = other.size() - pos : count)
	{
		raw_append(other.c_str() + pos, count);
	}

	template<typename C, typename T, typename A>
	basic_string2<C, T, A>::basic_string2(const_pointer str, size_type count, const allocator_type& alloc):
		base_type(alloc, count)
	{
		raw_append(str, count);
	}

	template<typename C, typename T, typename A>
	basic_string2<C, T, A>::basic_string2(const_pointer str, const allocator_type& alloc):
		this_type(str, traits_type::length(str), alloc)
	{
	}

	template<typename C, typename T, typename A>
	template<typename Iterator>
	basic_string2<C, T, A>::basic_string2(Iterator first, Iterator last, const allocator_type& alloc):
		this_type(first, last, alloc, simstd::pvt::iterator_category(first))
	{
	}

	template<typename C, typename T, typename A>
	basic_string2<C, T, A>::basic_string2(const this_type & other):
		base_type(other)
	{
	}

	template<typename C, typename T, typename A>
	basic_string2<C, T, A>::basic_string2(const this_type & other, const allocator_type& alloc):
		base_type(other, alloc)
	{
		if (base_type::m_impl != other.m_impl)
			raw_append(other.c_str(), other.size());
	}

	template<typename C, typename T, typename A>
	basic_string2<C, T, A>::basic_string2(this_type && other):
		base_type(other)
	{
	}

	template<typename C, typename T, typename A>
	basic_string2<C, T, A>::basic_string2(this_type&& other, const allocator_type& alloc):
		base_type(other, alloc)
	{
		if (base_type::m_impl != other.m_impl)
			raw_append(other.c_str(), other.size());
	}

//	template<typename C, typename T, typename A>
//	basic_string2<C, T, A>::basic_string2(std::initializer_list<value_type> ilist) :
//		m_data(string_impl::allocate(get_new_capacity(ilist.size())))
//	{
//		m_data->append(ilist.begin(), ilist.end());
//	}

//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::operator =(const this_type & right)
//	{
//		return assign(right);
//	}
//
//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::operator =(this_type && right)
//	{
//		clear();
//		swap(right);
//		return *this;
//	}
//
//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::operator =(const_pointer right)
//	{
//		return assign(right);
//	}
//
//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::operator =(value_type ch)
//	{
//		return assign(1, ch);
//	}
//
	template<typename C, typename T, typename A>
	bool basic_string2<C, T, A>::empty() const
	{
		return base_type::m_impl->get_size() == 0;
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::size_type basic_string2<C, T, A>::capacity() const
	{
		return base_type::m_impl->get_capa();
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::size_type basic_string2<C, T, A>::size() const
	{
		return base_type::m_impl->get_size();
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::size_type basic_string2<C, T, A>::length() const
	{
		return base_type::m_impl->get_size();
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::const_pointer basic_string2<C, T, A>::c_str() const
	{
		return base_type::m_impl->get_data();
	}

	template<typename C, typename T, typename A>
	void basic_string2<C, T, A>::swap(this_type & other)
	{
		using simstd::swap;
		swap(base_type::m_impl, other.m_impl);
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::const_iterator basic_string2<C, T, A>::cbegin() const
	{
		return const_iterator(c_str());
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::const_iterator basic_string2<C, T, A>::begin() const
	{
		return cbegin();
	}

//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::iterator basic_string2<C, T, A>::begin()
//	{
//		return iterator(_str());
//	}
//
	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::const_iterator basic_string2<C, T, A>::cend() const
	{
		return cbegin() + size();
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::const_iterator basic_string2<C, T, A>::end() const
	{
		return cbegin() + size();
	}

//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::iterator basic_string2<C, T, A>::end()
//	{
//		return begin() + size();
//	}
//
	template<typename C, typename T, typename A>
	void basic_string2<C, T, A>::clear()
	{
		if (!empty())
			split_and_clear(capacity());
	}

//	template<typename C, typename T, typename A>
//	void basic_string2<C, T, A>::reserve(size_type capa)
//	{
//		if (capacity() < capa) {
//			this_type(c_str(), size(), capa).swap(*this);
//		}
//	}
//
//	template<typename C, typename T, typename A>
//	int basic_string2<C, T, A>::compare(const_pointer str) const
//	{
//		return compare(c_str(), length(), str, traits_type::length(str));
//	}
//
//	template<typename C, typename T, typename A>
//	int basic_string2<C, T, A>::compare(const this_type & str) const
//	{
//		return compare(c_str(), length(), str.c_str(), str.length());
//	}
//
//	template<typename C, typename T, typename A>
//	int basic_string2<C, T, A>::compare(size_type pos, size_type count, const this_type & str) const
//	{
//		return compare(c_str() + pos, count, str.c_str(), str.length());
//	}
//
//	template<typename C, typename T, typename A>
//	int basic_string2<C, T, A>::compare(size_type pos, size_type count, const_pointer str) const
//	{
//		return compare(c_str() + pos, count, str, traits_type::length(str));
//	}
//
//	template<typename C, typename T, typename A>
//	int basic_string2<C, T, A>::compare(size_type pos, size_type count, const_pointer str, size_type len) const
//	{
//		return compare(c_str() + pos, count, str, len);
//	}
//
	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::append(size_type count, value_type ch)
	{
		split_and_copy(get_necessary_capacity(count));
		raw_append(count, ch);
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::append(const this_type & other)
	{
		return append(other.c_str(), other.size());
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::append(const this_type & other, size_type pos, size_type count)
	{
		return append(other.c_str() + pos, count == npos ? other.size() - pos : count);
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::append(const_pointer str, size_type count)
	{
		assert(str);
		if (count) {
			if (is_same_str(str)) {
				this_type tmp(c_str(), size(), get_necessary_capacity(count));
				tmp.append(str, count);
				tmp.swap(*this);
			} else {
				split_and_copy(get_necessary_capacity(count));
				raw_append(str, count);
			}
		}
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::append(const_pointer str)
	{
		return append(str, traits_type::length(str));
	}

	template<typename C, typename T, typename A>
	template<typename InputIt>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::append(InputIt first, InputIt last)
	{
		return _append(first, last, simstd::pvt::iterator_category(first));
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::assign(size_type count, value_type ch)
	{
		clear();
		return append(count, ch);
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::assign(const this_type & other)
	{
		clear();
		return append(other);
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::assign(const this_type & other, size_type pos, size_type count)
	{
		split_and_clear(count == npos ? other.size() - pos : count);
		return append(other, pos, count);
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::assign(this_type && other)
	{
		swap(other);
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::assign(const_pointer str, size_type count)
	{
		split_and_clear(count);
		return append(str, count);
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::assign(const_pointer str)
	{
		return assign(str, traits_type::length(str));
	}

	template<typename C, typename T, typename A>
	template<typename Iterator>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::assign(Iterator first, Iterator last)
	{
		return _assign(first, last, simstd::pvt::iterator_category(first));
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::iterator basic_string2<C, T, A>::insert(const_iterator pos, value_type ch)
	{
		typename simstd::iterator_traits<const_iterator>::difference_type posFirst = simstd::distance(cbegin(), pos);
		insert(posFirst, 0, 1, ch);
		return iterator(base_type::m_impl->get_data() + posFirst);
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::iterator basic_string2<C, T, A>::insert(iterator pos, size_type count, value_type ch)
	{
		typename simstd::iterator_traits<const_iterator>::difference_type posFirst = simstd::distance(begin(), pos);
		insert(posFirst, 0, count, ch);
		return iterator(base_type::m_impl->get_data() + posFirst);
	}

//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::insert(size_type index, size_type count, value_type ch)
//	{
//		return replace(index, 0, count, ch);
//	}
//
//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::insert(size_type index, const_pointer str)
//	{
//		return replace(index, 0, str, traits_type::length(str));
//	}
//
//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::insert(size_type index, const_pointer str, size_type count)
//	{
//		return replace(index, 0, str, count);
//	}
//
//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::insert(size_type index, const this_type & str)
//	{
//		return replace(index, 0, str.c_str(), str.length());
//	}
//
//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::insert(size_type index, const this_type & str, size_type index_str, size_type count)
//	{
//		return replace(index, 0, str.c_str(), count);
//	}
//
	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::operator +=(const this_type & other)
	{
		return append(other.c_str(), other.size());
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::operator +=(const_pointer str)
	{
		return append(str, traits_type::length(str));
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::operator +=(value_type ch)
	{
		return append(1, ch);
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::const_reference basic_string2<C, T, A>::operator [](size_type in) const
	{
		return *(c_str() + in);
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::reference basic_string2<C, T, A>::operator [](size_type in)
	{
		split(capacity());
		return *(base_type::m_impl->get_data() + in);
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::const_reference basic_string2<C, T, A>::at(size_type in) const
	{
		return *(c_str() + in);
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::reference basic_string2<C, T, A>::at(size_type in)
	{
		split(capacity());
		return *(base_type::m_impl->get_data() + in);
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::replace(size_type pos, size_type len, const this_type & other)
	{
		return replace(pos, len, other.c_str(), other.size());
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::replace(const_iterator first, const_iterator last, const this_type & other)
	{
		return replace(first, last, other.c_str(), other.size());
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::replace(size_type pos, size_type len, const this_type & other, size_type subpos, size_type sublen)
	{
		return replace(pos, len, &other[subpos], sublen == npos ? other.size() - subpos : sublen);
	}

	template<typename C, typename T, typename A>
	template<typename Iterator>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::replace(const_iterator first, const_iterator last, Iterator first2, Iterator last2)
	{
		// TODO
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::replace(size_type pos, size_type len, const_pointer str, size_type count)
	{
		assert(str);
//		printf("      (%Iu, %Iu): '%s'\n", size(), capacity(), c_str());
		pos = simstd::min(pos, size());
		len = simstd::min(len, size() - pos);
//		printf("      (pos: %Iu, len: %Iu)\n", pos, len);
		if (is_same_str(str) || base_type::m_impl->is_shared()) {
			this_type tmp(c_str(), pos, get_necessary_capacity(count - len));
//			printf("      (%Iu, %Iu): '%s'\n", tmp.size(), tmp.capacity(), tmp.c_str());
			tmp.append(str, count);
//			printf("      (%Iu, %Iu): '%s'\n", tmp.size(), tmp.capacity(), tmp.c_str());
			tmp.append(c_str() + pos, size() - pos);
//			printf("      (%Iu, %Iu): '%s'\n", tmp.size(), tmp.capacity(), tmp.c_str());
			tmp.swap(*this);
		} else {
			split(get_necessary_capacity(count - len));
//			printf("      (%Iu, %Iu): '%s'\n", size(), capacity(), c_str());
//			printf("replace before: '%s'\n", c_str());
			traits_type::move(_str() + pos + count, _str() + pos + len, size() - (pos + len) + 1);
//			traits_type::assign(_str() + pos, count, ' ');
//			printf("replace after : '%s'\n", c_str());
			traits_type::copy(_str() + pos, str, count);
//			printf("replace after : '%s'\n", c_str());
			base_type::m_impl->set_size(size() + count - len);
//			printf("      (%Iu, %Iu): '%s'\n", size(), capacity(), c_str());
		}
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::replace(const_iterator first, const_iterator last, const_pointer str, size_type count)
	{
		// TODO
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::replace(size_type pos, size_type len, const_pointer str)
	{
		return replace(pos, len, str, traits_type::length(str));
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::replace(const_iterator first, const_iterator last, const_pointer str)
	{
		return replace(first, last, str, traits_type::length(str));
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::replace(size_type pos, size_type len, size_type count, value_type ch)
	{
		return replace(pos, len, this_type(count, ch));
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::replace(const_iterator first, const_iterator last, size_type count, value_type ch)
	{
		return replace(first, last, this_type(count, ch));
	}

	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::this_type basic_string2<C, T, A>::substr(size_type pos, size_type n) const
	{
		return this_type(c_str() + pos, n, base_type::get_allocator());
	}

//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::erase(size_type pos, size_type count)
//	{
//		size_type posLast = (count == npos) ? size() : simstd::min(size(), pos + count);
//		erase(simstd::next(cbegin(), pos), simstd::next(cbegin(), posLast));
//		return *this;
//	}
//
//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::iterator basic_string2<C, T, A>::erase(const_iterator position)
//	{
//		return erase(position, simstd::next(position, 1));
//	}
//
//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::iterator basic_string2<C, T, A>::erase(const_iterator first, const_iterator last)
//	{
//		typename simstd::iterator_traits<const_iterator>::difference_type posFirst = simstd::distance(cbegin(), first);
//		typename simstd::iterator_traits<const_iterator>::difference_type posLast = simstd::distance(cbegin(), simstd::min(last, cend()));
//		if (is_shared()) {
//			this_type tmp(c_str(), posFirst, capacity());
//			tmp.append(c_str() + posLast, size() - posLast);
//			tmp.swap(*this);
//		} else {
//			traits_type::move(_str() + posFirst, _str() + posLast, size() - posLast + 1);
//			m_data->set_size(size() - (posLast - posFirst));
//		}
//		return iterator(_str() + posFirst);
//	}
//
//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::size_type basic_string2<C, T, A>::find(const this_type & str, size_type pos) const
//	{
//		return find(str.c_str(), pos, str.length());
//	}
//
//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::size_type basic_string2<C, T, A>::find(const_pointer str, size_type pos) const
//	{
//		return find(str, pos, traits_type::length(str));
//	}
//
//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::size_type basic_string2<C, T, A>::find(value_type ch, size_type pos) const
//	{
//		const_pointer find = traits_type::find(c_str() + pos, size(), ch);
//		return (find) ? find - c_str() : npos;
//	}
//
//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::size_type basic_string2<C, T, A>::rfind(const this_type & str, size_type pos) const
//	{
//		return rfind(str.c_str(), pos, str.length());
//	}
//
//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::size_type basic_string2<C, T, A>::rfind(const_pointer str, size_type pos, size_type count) const
//	{
//		pos = simstd::min(pos, size());
//		if (count != 0 && count <= pos) {
//			pos -= (count - 1);
//			while (pos > 0)
//				if (find(str, pos, count) != npos)
//					return pos;
//		}
//		return npos;
//	}
//
//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::size_type basic_string2<C, T, A>::rfind(const_pointer str, size_type pos) const
//	{
//		return rfind(str, pos, traits_type::length(str));
//	}
//
//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::size_type basic_string2<C, T, A>::rfind(value_type ch, size_type pos) const
//	{
//		pos = simstd::min(pos, size());
//		while (pos > 0)
//			if (traits_type::eq(at(--pos), ch))
//				return pos;
//		return npos;
//	}
//
//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::size_type basic_string2<C, T, A>::find_first_of(const this_type & str, size_type pos) const
//	{
//		for (; pos < size(); ++pos)
//			if (traits_type::find(str.c_str(), str.size(), at(pos)))
//				return pos;
//		return npos;
//	}
//
//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::size_type basic_string2<C, T, A>::find_last_of(const this_type & str, size_type pos) const
//	{
//		pos = simstd::min(pos, size());
//		while (pos > 0)
//			if (traits_type::find(str.c_str(), str.size(), at(--pos)))
//				return pos;
//		return npos;
//	}
//
//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::size_type basic_string2<C, T, A>::find_first_not_of(const this_type & str, size_type pos) const
//	{
//		for (; pos < size(); ++pos)
//			if (traits_type::find(str.c_str(), str.size(), at(pos)) == nullptr)
//				return pos;
//		return npos;
//	}
//
//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::size_type basic_string2<C, T, A>::find_last_not_of(const this_type & str, size_type pos) const
//	{
//		pos = simstd::min(pos, size());
//		while (pos > 0)
//			if (traits_type::find(str.c_str(), str.size(), at(--pos)) == nullptr)
//				return pos;
//		return npos;
//	}
//

	template<typename C, typename T, typename A>
	basic_string2<C, T, A>::basic_string2(const A & alloc, const_pointer str, size_type count, size_type capacity) :
		base_type(alloc, capacity)
	{
		if (count)
			raw_append(str, count);
	}

	template<typename C, typename T, typename A>
	template<typename InputIt>
	basic_string2<C, T, A>::basic_string2(InputIt first, InputIt last, const allocator_type& alloc, simstd::input_iterator_tag):
		base_type(alloc)
	{
		append(first, last);
	}

	template<typename C, typename T, typename A>
	template<typename ForwardIt>
	basic_string2<C, T, A>::basic_string2(ForwardIt first, ForwardIt last, const allocator_type& alloc, simstd::forward_iterator_tag):
		base_type(alloc, simstd::distance(first, last))
	{
		raw_append(first, last);
	}

	template<typename C, typename T, typename A>
	void basic_string2<C, T, A>::raw_append(size_type count, value_type ch)
	{
		auto ptr = base_type::m_impl->get_data() + size();
		traits_type::assign(ptr, count, ch);
		base_type::m_impl->set_size(size() + count);
	}

	template<typename C, typename T, typename A>
	void basic_string2<C, T, A>::raw_append(const_pointer str, size_type count)
	{
		auto ptr = base_type::m_impl->get_data() + size();
		traits_type::move(ptr, str, count);
		base_type::m_impl->set_size(size() + count);
	}

	template<typename C, typename T, typename A>
	template<typename InputIt>
	void basic_string2<C, T, A>::raw_append(InputIt first, InputIt last)
	{
		auto ptr = base_type::m_impl->get_data() + size();
		auto endIt = simstd::copy(first, last, ptr);
		base_type::m_impl->set_size(size() + simstd::distance(ptr, endIt));
	}

	template<typename C, typename T, typename A>
	template<typename InputIt>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::_append(InputIt first, InputIt last, simstd::input_iterator_tag)
	{
		// TODO
		return *this;
	}

	template<typename C, typename T, typename A>
	template<typename ForwardIt>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::_append(ForwardIt first, ForwardIt last, simstd::forward_iterator_tag)
	{
		split_and_copy(size() + simstd::distance(first, last));
		return raw_append(first, last);
	}

	template<typename C, typename T, typename A>
	template<typename InputIt>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::_assign(InputIt first, InputIt last, simstd::input_iterator_tag)
	{
		split_and_clear(capacity());
		return _append(first, last, simstd::input_iterator_tag());
	}

	template<typename C, typename T, typename A>
	template<typename ForwardIt>
	typename basic_string2<C, T, A>::this_type & basic_string2<C, T, A>::_assign(ForwardIt first, ForwardIt last, simstd::forward_iterator_tag)
	{
		split_and_clear(simstd::distance(first, last));
		return raw_append(first, last);
	}

//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::pointer basic_string2<C, T, A>::_str() const
//	{
//		return m_data->get_str_data();
//	}
//
	template<typename C, typename T, typename A>
	typename basic_string2<C, T, A>::size_type basic_string2<C, T, A>::get_necessary_capacity(ptrdiff_t addToSize) const
	{
		if (capacity() < (size() + addToSize))
			return size() + simstd::max((ptrdiff_t)size(), addToSize);
		return capacity();
	}

	template<typename C, typename T, typename A>
	void basic_string2<C, T, A>::split_and_clear(size_type minCapacity)
	{
		LogDebug(L"minCapacity: %Iu\n", minCapacity);
		if (capacity() < minCapacity)
			this_type(base_type::get_allocator(), nullptr, 0, minCapacity).swap(*this);
		else if (base_type::m_impl->is_shared())
			this_type(base_type::get_allocator(), nullptr, 0, capacity()).swap(*this);
		else
			base_type::m_impl->set_size(0);
	}

	template<typename C, typename T, typename A>
	void basic_string2<C, T, A>::split_and_copy(size_type minCapacity)
	{
		LogDebug(L"minCapacity: %Iu\n", minCapacity);
		if (capacity() < minCapacity)
			this_type(base_type::get_allocator(), c_str(), size(), minCapacity).swap(*this);
		else if (base_type::m_impl->is_shared())
			this_type(base_type::get_allocator(), c_str(), size(), capacity()).swap(*this);
	}

//	template<typename C, typename T, typename A>
//	int basic_string2<C, T, A>::compare(const_pointer str1, size_type count1, const_pointer str2, size_type count2)
//	{
//		int result = traits_type::compare(str1, str2, simstd::min(count1, count2));
//		if (result != 0)
//			return result;
//		if (count1 < count2)
//			return -1;
//		if (count1 > count2)
//			return 1;
//		return 0;
//	}
//
//	template<typename C, typename T, typename A>
//	typename basic_string2<C, T, A>::size_type basic_string2<C, T, A>::find(const_pointer str, size_type pos, size_type length) const
//	{
//		const size_type size = this->size();
//		const_pointer data = this->c_str();
//
//		if (length == 0)
//			return pos <= size ? pos : npos;
//
//		if (length <= size)
//		{
//			for (; pos <= size - length; ++pos)
//				if (traits_type::eq(data[pos], str[0]) && traits_type::compare(data + pos + 1, str + 1, length - 1) == 0)
//					return pos;
//		}
//		return npos;
//	}
//
	template<typename C, typename T, typename A>
	bool basic_string2<C, T, A>::is_same_str(const_pointer str) const
	{
		return c_str() <= str && str < (c_str() + size()); // append to itself
	}

//	template<typename C, typename T, typename A>
//	bool basic_string2<C, T, A>::is_shared() const
//	{
//		return m_data->count_ref() > 1;
//	}
//
//	///=============================================================================================
//	template<typename C, typename T, typename A>
//	basic_string2<C, T, A> operator +(const basic_string2<C, T, A> & left, const basic_string2<C, T, A> & right)
//	{
//		return basic_string2<C, T, A>(left.c_str(), left.size(), left.size() + right.size()) += right;
//	}
//
//	template<typename C, typename T, typename A>
//	basic_string2<C, T, A> operator +(const C * left, const basic_string2<C, T, A> & right)
//	{
//		typedef typename basic_string2<C, T, A>::traits_type traits_type;
//		typename basic_string2<C, T, A>::size_type length = traits_type::length(left);
//		return basic_string2<C, T, A>(left, length, length + right.size()) += right;
//	}
//
//	template<typename C, typename T, typename A>
//	basic_string2<C, T, A> operator +(C left, const basic_string2<C, T, A> & right)
//	{
//		return basic_string2<C, T, A>(&left, 1, 1 + right.size()) += right;
//	}
//
//	template<typename C, typename T, typename A>
//	basic_string2<C, T, A> operator +(const basic_string2<C, T, A> & left, const C * right)
//	{
//		typedef typename basic_string2<C, T, A>::traits_type traits_type;
//		return basic_string2<C, T, A>(left.c_str(), left.size(), left.size() + traits_type::length(right)) += right;
//	}
//
//	template<typename C, typename T, typename A>
//	basic_string2<C, T, A> operator +(const basic_string2<C, T, A> & left, C right)
//	{
//		return basic_string2<C, T, A>(left.c_str(), left.size(), left.size() + 1) += right;
//	}
//
//	template<typename C, typename T, typename A>
//	bool operator ==(const basic_string2<C, T, A> & left, const basic_string2<C, T, A> & right)
//	{
//		return basic_string2<C, T, A>::compare(left.c_str(), left.size(), right.c_str(), right.size()) == 0;
//	}
//
//	template<typename C, typename T, typename A>
//	bool operator ==(const C * left, const basic_string2<C, T, A> & right)
//	{
//		typedef typename basic_string2<C, T, A>::traits_type traits_type;
//		return basic_string2<C, T, A>::compare(left, traits_type::length(left), right.c_str(), right.size()) == 0;
//	}
//
//	template<typename C, typename T, typename A>
//	bool operator ==(const basic_string2<C, T, A> & left, const C * right)
//	{
//		typedef typename basic_string2<C, T, A>::traits_type traits_type;
//		return basic_string2<C, T, A>::compare(left.c_str(), left.size(), right, traits_type::length(right)) == 0;
//	}
//
//	template<typename C, typename T, typename A>
//	bool operator !=(const basic_string2<C, T, A> & left, const basic_string2<C, T, A> & right)
//	{
//		return !operator ==(left, right);
//	}
//
//	template<typename C, typename T, typename A>
//	bool operator !=(const C * left, const basic_string2<C, T, A> & right)
//	{
//		return !operator ==(left, right);
//	}
//
//	template<typename C, typename T, typename A>
//	bool operator !=(const basic_string2<C, T, A> & left, const C * right)
//	{
//		return !operator ==(left, right);
//	}
//
//	template<typename C, typename T, typename A>
//	bool operator <(const basic_string2<C, T, A> & left, const basic_string2<C, T, A> & right)
//	{
//		return basic_string2<C, T, A>::compare(left.c_str(), left.size(), right.c_str(), right.size()) < 0;
//	}
//
//	template<typename C, typename T, typename A>
//	bool operator <(const C * left, const basic_string2<C, T, A> & right)
//	{
//		typedef typename basic_string2<C, T, A>::traits_type traits_type;
//		return basic_string2<C, T, A>::compare(left, traits_type::length(left), right.c_str(), right.size()) < 0;
//	}
//
//	template<typename C, typename T, typename A>
//	bool operator <(const basic_string2<C, T, A> & left, const C * right)
//	{
//		typedef typename basic_string2<C, T, A>::traits_type traits_type;
//		return basic_string2<C, T, A>::compare(left.c_str(), left.size(), right, traits_type::length(right)) < 0;
//	}
//
//	template<typename C, typename T, typename A>
//	bool operator <=(const basic_string2<C, T, A> & left, const basic_string2<C, T, A> & right)
//	{
//		return basic_string2<C, T, A>::compare(left.c_str(), left.size(), right.c_str(), right.size()) <= 0;
//	}
//
//	template<typename C, typename T, typename A>
//	bool operator <=(const C * left, const basic_string2<C, T, A> & right)
//	{
//		typedef typename basic_string2<C, T, A>::traits_type traits_type;
//		return basic_string2<C, T, A>::compare(left, traits_type::length(left), right.c_str(), right.size()) <= 0;
//	}
//
//	template<typename C, typename T, typename A>
//	bool operator <=(const basic_string2<C, T, A> & left, const C * right)
//	{
//		typedef typename basic_string2<C, T, A>::traits_type traits_type;
//		return basic_string2<C, T, A>::compare(left.c_str(), left.size(), right, traits_type::length(right)) <= 0;
//	}
//
//	template<typename C, typename T, typename A>
//	bool operator >(const basic_string2<C, T, A> & left, const basic_string2<C, T, A> & right)
//	{
//		return basic_string2<C, T, A>::compare(left.c_str(), left.size(), right.c_str(), right.size()) > 0;
//	}
//
//	template<typename C, typename T, typename A>
//	bool operator >(const C * left, const basic_string2<C, T, A> & right)
//	{
//		typedef typename basic_string2<C, T, A>::traits_type traits_type;
//		return basic_string2<C, T, A>::compare(left, traits_type::length(left), right.c_str(), right.size()) > 0;
//	}
//
//	template<typename C, typename T, typename A>
//	bool operator >(const basic_string2<C, T, A> & left, const C * right)
//	{
//		typedef typename basic_string2<C, T, A>::traits_type traits_type;
//		return basic_string2<C, T, A>::compare(left.c_str(), left.size(), right, traits_type::length(right)) > 0;
//	}
//
//	template<typename C, typename T, typename A>
//	bool operator >=(const basic_string2<C, T, A> & left, const basic_string2<C, T, A> & right)
//	{
//		return basic_string2<C, T, A>::compare(left.c_str(), left.size(), right.c_str(), right.size()) >= 0;
//	}
//
//	template<typename C, typename T, typename A>
//	bool operator >=(const C * left, const basic_string2<C, T, A> & right)
//	{
//		typedef typename basic_string2<C, T, A>::traits_type traits_type;
//		return basic_string2<C, T, A>::compare(left, traits_type::length(left), right.c_str(), right.size()) >= 0;
//	}
//
//	template<typename C, typename T, typename A>
//	bool operator >=(const basic_string2<C, T, A> & left, const C * right)
//	{
//		typedef typename basic_string2<C, T, A>::traits_type traits_type;
//		return basic_string2<C, T, A>::compare(left.c_str(), left.size(), right, traits_type::length(right)) >= 0;
//	}
//
//	template<typename C, typename T, typename A>
//	void swap(basic_string2<C, T, A> & left, basic_string2<C, T, A> & right)
//	{
//		left.swap(right);
//	}

	///=============================================================================================
//	typedef basic_string<char>    string;
//	typedef basic_string<wchar_t> wstring;

}

//typedef simstd::string  astring;
//typedef simstd::wstring ustring;

#endif