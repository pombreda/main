#ifndef SARALIB_STL_VECTOR_HPP_
#define SARALIB_STL_VECTOR_HPP_

#include "impl_types.hpp"
#include "impl_algorithm_base.hpp"
#include "impl_iterator.hpp"
#include "impl_memory.hpp"

namespace sarastd {
	template<typename Type, typename Allocator_ = sarastd::allocator<Type> >
	class vector {
		typedef vector this_type;

	public:
		typedef Type value_type;
		typedef sarastd::allocator<Type> allocator_type; // allocator is always standard
		typedef sarastd::size_t size_type;
		typedef allocator_traits<allocator_type> alloc_traits;
		typedef typename alloc_traits::pointer pointer;
		typedef typename alloc_traits::const_pointer const_pointer;
		typedef typename alloc_traits::reference reference;
		typedef typename alloc_traits::const_reference const_reference;
		typedef typename alloc_traits::difference_type difference_type;
		typedef sarastd::normal_iterator<pointer> iterator;
		typedef sarastd::normal_iterator<const_pointer> const_iterator;
		typedef sarastd::reverse_iterator<iterator> reverse_iterator;
		typedef sarastd::reverse_iterator<const_iterator> const_reverse_iterator;

	public:
		~vector();

		vector();
		explicit vector(size_type n);
		vector(size_type n, const value_type& val);
		vector(const this_type& other);

		template<typename InputIt>
		vector(InputIt first, InputIt last);

		this_type& operator =(const this_type& other);

		template<typename InputIt>
		void assign(InputIt first, InputIt last);
		void assign(size_type n, const value_type& value);

		reference operator [](size_type pos);
		const_reference operator [](size_type pos) const;
		reference at(size_type pos);
		const_reference at(size_type pos) const;

		reference front();
		const_reference front() const;
		reference back();
		const_reference back() const;
		value_type* data();
		const value_type* data() const;

		iterator begin();
		const_iterator begin() const;
		const_iterator cbegin() const;
		iterator end();
		const_iterator end() const;
		const_iterator cend() const;
		reverse_iterator rbegin();
		const_reverse_iterator rbegin() const;
		const_reverse_iterator crbegin() const;
		reverse_iterator rend();
		const_reverse_iterator rend() const;
		const_reverse_iterator crend() const;

		bool empty() const;
		size_type size() const;
		size_type max_size() const;
		void reserve(size_type size);
		size_type capacity() const;
		void shrink_to_fit();

		void clear();

		template<typename InputIt>
		iterator insert(iterator pos, InputIt first, InputIt last);
		iterator insert(iterator pos, const value_type& value);
		iterator insert(iterator pos, size_type n, const value_type& value);

		iterator erase(iterator first, iterator last);
		iterator erase(iterator pos);

		void push_back(const value_type& value);
		void pop_back();

		void resize(size_type count, const value_type& value);
		void resize(size_type count);

		void swap(this_type& other);

	private:
		template<typename InputIt>
		void _insert_back(InputIt first, InputIt last, sarastd::input_iterator_tag);

		template<typename ForwardIt>
		void _insert_back(ForwardIt first, ForwardIt last, sarastd::forward_iterator_tag);

		template<typename InputIt>
		iterator _insert(iterator pos, InputIt first, InputIt last, sarastd::input_iterator_tag);

		template<typename ForwardIt>
		iterator _insert(iterator pos, ForwardIt first, ForwardIt last, sarastd::forward_iterator_tag);

		struct impl: private allocator_type {
			typedef impl this_type;
			typedef vector::value_type value_type;
			typedef vector::allocator_type allocator_type;
			typedef vector::size_type size_type;
			typedef vector::pointer pointer;

			pointer begin;
			pointer end;
			pointer end_of_storage;

			~impl();
			impl();
			impl(size_type n);
			impl(size_type n, const value_type& value);
			impl(size_type n, pointer first, pointer last);
			void swap(this_type& other);
			void destroy_objects(pointer first, pointer last);
			void reserve(size_type newCapacity);
			void adjust_capacity(size_type add);
			bool check_capacity(size_type add) const;
			size_type get_new_capacity(size_type add) const;
			size_type capacity() const;
			size_type size() const;
			size_type max_size() const;
		private:
			void create_storage(size_type n);
		} m_impl;
	};

	///=============================================================================================
	template<typename Type, typename Allocator>
	bool operator ==(const vector<Type, Allocator>& lhs, const vector<Type, Allocator>& rhs)
	{
		return (lhs.size() == rhs.size() && sarastd::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}

	template<typename Type, typename Allocator>
	bool operator !=(const vector<Type, Allocator>& lhs, const vector<Type, Allocator>& rhs)
	{
		return !(lhs == rhs);
	}

	template<typename Type, typename Allocator>
	bool operator <(const vector<Type, Allocator>& lhs, const vector<Type, Allocator>& rhs)
	{
		return sarastd::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template<typename Type, typename Allocator>
	bool operator >(const vector<Type, Allocator>& lhs, const vector<Type, Allocator>& rhs)
	{
		return rhs < lhs;
	}

	template<typename Type, typename Allocator>
	bool operator <=(const vector<Type, Allocator>& lhs, const vector<Type, Allocator>& rhs)
	{
		return !(rhs < lhs);
	}

	template<typename Type, typename Allocator>
	bool operator >=(const vector<Type, Allocator>& lhs, const vector<Type, Allocator>& rhs)
	{
		return !(lhs < rhs);
	}

	template<typename Type, typename Allocator>
	void swap(vector<Type, Allocator>& lhs, vector<Type, Allocator>& rhs)
	{
		lhs.swap(rhs);
	}

	///=============================================================================================
	template<typename Type, typename Allocator>
	vector<Type, Allocator>::~vector()
	{
	}

	template<typename Type, typename Allocator>
	vector<Type, Allocator>::vector() :
		m_impl()
	{
	}

	template<typename Type, typename Allocator>
	vector<Type, Allocator>::vector(size_type n) :
		m_impl(n, value_type())
	{
	}

	template<typename Type, typename Allocator>
	vector<Type, Allocator>::vector(size_type n, const value_type& val) :
		m_impl(n, val)
	{
	}

	template<typename Type, typename Allocator>
	template<typename InputIt>
	vector<Type, Allocator>::vector(InputIt first, InputIt last) :
		m_impl()
	{
		_insert_back(first, last, _iterator_category(first));
	}

	template<typename Type, typename Allocator>
	vector<Type, Allocator>::vector(const this_type& other) :
		m_impl(other.capacity(), other.m_impl.begin, other.m_impl.end)
	{
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::this_type& vector<Type, Allocator>::operator =(const this_type& other)
	{
		this_type(other).swap(*this);
		return *this;
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::assign(size_type n, const value_type& value)
	{
		this_type(n, value).swap(*this);
	}

	template<typename Type, typename Allocator>
	template<typename InputIt>
	void vector<Type, Allocator>::assign(InputIt first, InputIt last)
	{
		this_type(first, last).swap(*this);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::reference vector<Type, Allocator>::operator [](size_type pos)
	{
		return *(m_impl.begin + pos);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_reference vector<Type, Allocator>::operator [](size_type pos) const
	{
		return (*this)[pos];
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::reference vector<Type, Allocator>::at(size_type pos)
	{
		return *(m_impl.begin + pos);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_reference vector<Type, Allocator>::at(size_type pos) const
	{
		return this->at(pos);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::reference vector<Type, Allocator>::front()
	{
		return *(m_impl.begin);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_reference vector<Type, Allocator>::front() const
	{
		return *(m_impl.begin);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::reference vector<Type, Allocator>::back()
	{
		return *(m_impl.end - 1);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_reference vector<Type, Allocator>::back() const
	{
		return *(m_impl.end - 1);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::value_type* vector<Type, Allocator>::data()
	{
		return m_impl.begin;
	}

	template<typename Type, typename Allocator>
	const typename vector<Type, Allocator>::value_type* vector<Type, Allocator>::data() const
	{
		return m_impl.begin;
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::begin()
	{
		return iterator(m_impl.begin);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_iterator vector<Type, Allocator>::begin() const
	{
		return cbegin();
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_iterator vector<Type, Allocator>::cbegin() const
	{
		return const_iterator(m_impl.begin);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::end()
	{
		return iterator(m_impl.end);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_iterator vector<Type, Allocator>::end() const
	{
		return cend();
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_iterator vector<Type, Allocator>::cend() const
	{
		return const_iterator(m_impl.end);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::reverse_iterator vector<Type, Allocator>::rbegin()
	{
		return reverse_iterator(iterator(m_impl.end));
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_reverse_iterator vector<Type, Allocator>::rbegin() const
	{
		return crbegin();
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_reverse_iterator vector<Type, Allocator>::crbegin() const
	{
		return const_reverse_iterator(const_iterator(m_impl.end));
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::reverse_iterator vector<Type, Allocator>::rend()
	{
		return reverse_iterator(iterator(m_impl.begin));
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_reverse_iterator vector<Type, Allocator>::rend() const
	{
		return crend();
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_reverse_iterator vector<Type, Allocator>::crend() const
	{
		return const_reverse_iterator(const_iterator(m_impl.begin));
	}

	template<typename Type, typename Allocator>
	bool vector<Type, Allocator>::empty() const
	{
		return m_impl.end == m_impl.begin;
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::size_type vector<Type, Allocator>::size() const
	{
		return m_impl.size();
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::size_type vector<Type, Allocator>::max_size() const
	{
		return m_impl.max_size();
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::reserve(size_type size)
	{
		m_impl.reserve(size);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::size_type vector<Type, Allocator>::capacity() const
	{
		return m_impl.end_of_storage - m_impl.begin;
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::shrink_to_fit()
	{
		this_type(*this).swap(*this);
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::clear()
	{
		if (!empty())
			this_type().swap(*this);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::insert(iterator pos, const value_type& value)
	{
		return insert(pos, (size_type)1, value);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::insert(iterator pos, size_type n, const value_type& value)
	{
		if (m_impl.check_capacity(n)) {
			iterator oldEnd(end());
			size_type elems_between = oldEnd - pos;
			if (elems_between < n) {
				sarastd::uninitialized_fill_n(oldEnd, n - elems_between, value);
				sarastd::uninitialized_copy(pos, oldEnd, oldEnd + n - elems_between);
				sarastd::fill_n(pos, elems_between, value);
			} else {
				sarastd::uninitialized_copy(end() - n, end(), end());
				sarastd::copy_backward(pos, end() - n, end());
				sarastd::fill(pos, pos + n, value);
			}
			m_impl.end += n;
			return pos;
		}
		impl newImpl(m_impl.get_new_capacity(n), m_impl.begin, &*pos);
		iterator ret(newImpl.end);
		sarastd::uninitialized_fill_n(ret, n, value);
		newImpl.end += n;
		sarastd::uninitialized_copy(pos, end(), iterator(newImpl.end));
		newImpl.end += (end() - pos);
		newImpl.swap(m_impl);
		return ret;
	}

	template<typename Type, typename Allocator>
	template<typename InputIt>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::insert(iterator pos, InputIt first, InputIt last)
	{
		return _insert(pos, first, last, _iterator_category(first));
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::erase(iterator first, iterator last)
	{
		sarastd::copy(last, end(), first);
		size_type n = sarastd::distance(first, last);
		m_impl.destroy_objects(m_impl.end - n, m_impl.end);
		m_impl.end -= n;
		return first;
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::erase(iterator pos)
	{
		return erase(pos, pos + 1);
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::push_back(const value_type& value)
	{
		m_impl.adjust_capacity(1);
		sarastd::uninitialized_fill_n(end(), 1, value);
		++m_impl.end;
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::pop_back()
	{
		erase(end() - 1);
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::resize(size_type count, const value_type& value)
	{
		if (size() < count) {
			reserve(count);
			sarastd::uninitialized_fill_n(end(), count - size(), value);
			m_impl.end += (count - size());
		}
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::resize(size_type count)
	{
		resize(count, value_type());
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::swap(this_type& other)
	{
		m_impl.swap(other.m_impl);
	}

	template<typename Type, typename Allocator>
	template<typename InputIt>
	void vector<Type, Allocator>::_insert_back(InputIt first, InputIt last, sarastd::input_iterator_tag)
	{
		sarastd::copy(first, last, sarastd::back_inserter(*this));
	}

	template<typename Type, typename Allocator>
	template<typename ForwardIt>
	void vector<Type, Allocator>::_insert_back(ForwardIt first, ForwardIt last, sarastd::forward_iterator_tag)
	{
		m_impl.adjust_capacity(sarastd::distance(first, last));
		_insert_back(first, last, sarastd::input_iterator_tag());
	}

	template<typename Type, typename Allocator>
	template<typename InputIt>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::_insert(iterator pos, InputIt first, InputIt last, sarastd::input_iterator_tag)
	{
		size_type distance = sarastd::distance(begin(), pos);
		sarastd::copy(first, last, sarastd::inserter(*this, pos));
		return begin() + distance;
	}

	template<typename Type, typename Allocator>
	template<typename ForwardIt>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::_insert(iterator pos, ForwardIt first, ForwardIt last, sarastd::forward_iterator_tag)
	{
		size_type distance = sarastd::distance(begin(), pos);
		size_type n = sarastd::distance(first, last);
		iterator oldEnd(end());
		if (m_impl.check_capacity(n)) {
			size_type elems_between = oldEnd - pos;
			if (elems_between < n) {
				ForwardIt mid(first);
				sarastd::advance(mid, elems_between);
				sarastd::uninitialized_copy(mid, last, oldEnd);
				sarastd::uninitialized_copy(pos, oldEnd, oldEnd + n - elems_between);
				sarastd::copy(first, mid, pos);
			} else {
				sarastd::uninitialized_copy(oldEnd - n, oldEnd, oldEnd);
				sarastd::copy_backward(pos, oldEnd - n, oldEnd);
				sarastd::copy(first, last, pos);
			}
			m_impl.end += n;
			return pos;
		}
		impl newImpl(m_impl.get_new_capacity(n), m_impl.begin, &*pos);
		iterator ret(newImpl.end);
		sarastd::uninitialized_copy(first, last, ret);
		newImpl.end += n;
		sarastd::uninitialized_copy(pos, oldEnd, ret + n);
		newImpl.end += (oldEnd - pos);
		newImpl.swap(m_impl);
		return ret;
	}

	///=============================================================================================
	template<typename Type, typename Allocator>
	vector<Type, Allocator>::impl::~impl()
	{
		if (begin)
			allocator_type::deallocate(begin, capacity());
	}

	template<typename Type, typename Allocator>
	vector<Type, Allocator>::impl::impl() :
		begin(0), end(0), end_of_storage(0)
	{
	}

	template<typename Type, typename Allocator>
	vector<Type, Allocator>::impl::impl(size_type n) :
		begin(0), end(0), end_of_storage(0)
	{
		create_storage(n);
	}

	template<typename Type, typename Allocator>
	vector<Type, Allocator>::impl::impl(size_type n, const value_type& value) :
		begin(0), end(0), end_of_storage(0)
	{
		create_storage(n);
		sarastd::uninitialized_fill_n(begin, n, value);
		end += n;
	}

	template<typename Type, typename Allocator>
	vector<Type, Allocator>::impl::impl(size_type n, pointer first, pointer last) :
		begin(0), end(0), end_of_storage(0)
	{
		create_storage(n);
		sarastd::uninitialized_copy(first, last, end);
		end += (last - first);
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::impl::swap(this_type& other)
	{
		using sarastd::swap;
		swap(begin, other.begin);
		swap(end, other.end);
		swap(end_of_storage, other.end_of_storage);
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::impl::create_storage(size_type n)
	{
		begin = (n) ? allocator_type::allocate(n) : 0;
		end = begin;
		end_of_storage = begin + n;
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::impl::destroy_objects(pointer first, pointer last)
	{
		for (; first != last; ++first)
			allocator_type::destroy(first);
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::impl::reserve(size_type newCapacity)
	{
		if (capacity() < newCapacity)
			this_type(newCapacity, begin, end).swap(*this);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::impl::size_type vector<Type, Allocator>::impl::size() const
	{
		return end - begin;
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::impl::size_type vector<Type, Allocator>::impl::max_size() const
	{
		return 0xFFFFFFFFu;
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::impl::size_type vector<Type, Allocator>::impl::capacity() const
	{
		return end_of_storage - begin;
	}

	template<typename Type, typename Allocator>
	bool vector<Type, Allocator>::impl::check_capacity(size_type add) const
	{
		if ((size() + add) > capacity())
			return false;
		return true;
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::impl::size_type vector<Type, Allocator>::impl::get_new_capacity(size_type add) const
	{
		return size() + sarastd::max(size_type(4), sarastd::max(size(), add));
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::impl::adjust_capacity(size_type add)
	{
		if (!check_capacity(add))
			reserve(get_new_capacity(add));
	}

}

#endif
