﻿#ifndef BASIS_SIMSTD_VECTOR_INTERFACE_HPP_
#define BASIS_SIMSTD_VECTOR_INTERFACE_HPP_

namespace simstd {

	template<typename Type, typename Allocator>
	class vector: private pvt::vector_base<Type, Allocator> {
		using this_type = vector;
		using base_type = pvt::vector_base<Type, Allocator>;
		using base_type::impl;
		using base_type::get_base_allocator;

	public:
		using value_type = Type ;
		using allocator_type = Allocator;
		using size_type = size_t;
		using alloc_traits = allocator_traits<allocator_type>;
		using pointer = typename alloc_traits::pointer;
		using const_pointer = typename alloc_traits::const_pointer;
		using reference = value_type&;
		using const_reference = const value_type&;
		using difference_type = typename alloc_traits::difference_type;
		using iterator = simstd::pvt::normal_iterator<pointer>;
		using const_iterator = simstd::pvt::normal_iterator<const_pointer>;
		using reverse_iterator = simstd::reverse_iterator<iterator>;
		using const_reverse_iterator = simstd::reverse_iterator<const_iterator>;

	public:
		~vector();

		vector();
		explicit vector(const allocator_type& allocator);
		vector(size_type n, const value_type& val, const allocator_type& allocator = allocator_type());
		explicit vector(size_type n, const allocator_type& allocator = allocator_type());
		template<typename InputIterator>
		vector(InputIterator first, InputIterator last, const allocator_type& allocator = allocator_type());
		vector(const this_type& other);
		vector(const this_type& other, const allocator_type& allocator);
		vector(this_type&& other);
		vector(this_type&& other, const allocator_type& allocator);
//		vector(simstd::initializer_list<Type> init, const Allocator& allocator = allocator_type());

		this_type& operator =(const this_type& other);
		this_type& operator =(this_type&& other);

		template<typename InputIterator>
		void assign(InputIterator first, InputIterator last);
		void assign(size_type n, const value_type& value);

		reference       at(size_type pos);
		const_reference at(size_type pos) const;
		reference       operator [](size_type pos);
		const_reference operator [](size_type pos) const;

		reference         front();
		const_reference   front() const;
		reference         back();
		const_reference   back() const;
		value_type*       data();
		const value_type* data() const;

		iterator               begin();
		const_iterator         begin() const;
		const_iterator         cbegin() const;
		iterator               end();
		const_iterator         end() const;
		const_iterator         cend() const;
		reverse_iterator       rbegin();
		const_reverse_iterator rbegin() const;
		const_reverse_iterator crbegin() const;
		reverse_iterator       rend();
		const_reverse_iterator rend() const;
		const_reverse_iterator crend() const;

		bool      empty() const;
		size_type size() const;
		size_type max_size() const;
		void      reserve(size_type new_capa);
		size_type capacity() const;
		void      shrink_to_fit();

		void      clear();

		template<typename InputIterator>
		iterator insert(const_iterator pos, InputIterator first, InputIterator last);
		iterator insert(const_iterator pos, const value_type& value);
		iterator insert(const_iterator pos, size_type n, const value_type& value);
		iterator insert(const_iterator pos, value_type&& value);

		iterator erase(const_iterator pos);
		iterator erase(const_iterator first, const_iterator last);

		template<typename... Args>
		iterator emplace(const_iterator pos, Args&&... args);

		void     push_back(const value_type& value);
		void     push_back(value_type&& value);

		template<typename... Args>
		void     emplace_back(Args&&... args);

		void     pop_back();

		void     resize(size_type count);
		void     resize(size_type count, const value_type& value);

		void     swap(this_type& other);

	private:
		void _reserve(size_type new_capa);

		iterator _erase(const_iterator first, const_iterator last);

		template<typename... Args>
		iterator _emplace(const_iterator pos, Args&&... args);

		template<typename... Args>
		void _emplace_back(Args&&... args);

		template<typename InputIterator>
		void _insert_back(InputIterator first, InputIterator last, simstd::input_iterator_tag);

		template<typename ForwardIterator>
		void _insert_back(ForwardIterator first, ForwardIterator last, simstd::forward_iterator_tag);

		iterator _insert(const_iterator pos, size_type n, const value_type& value);

		template<typename InputIterator>
		iterator _insert(const_iterator pos, InputIterator first, InputIterator last, simstd::input_iterator_tag);

		template<typename ForwardIterator>
		iterator _insert(const_iterator pos, ForwardIterator first, ForwardIterator last, simstd::forward_iterator_tag);
	};

	///=============================================================================================
	template<typename Type, typename Allocator>
	bool operator ==(const vector<Type, Allocator>& lhs, const vector<Type, Allocator>& rhs)
	{
		return (lhs.size() == rhs.size() && simstd::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}

	template<typename Type, typename Allocator>
	bool operator !=(const vector<Type, Allocator>& lhs, const vector<Type, Allocator>& rhs)
	{
		return simstd::rel_ops::operator !=(lhs, rhs);
	}

	template<typename Type, typename Allocator>
	bool operator <(const vector<Type, Allocator>& lhs, const vector<Type, Allocator>& rhs)
	{
		return simstd::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template<typename Type, typename Allocator>
	bool operator >(const vector<Type, Allocator>& lhs, const vector<Type, Allocator>& rhs)
	{
		return simstd::rel_ops::operator >(lhs, rhs);
	}

	template<typename Type, typename Allocator>
	bool operator <=(const vector<Type, Allocator>& lhs, const vector<Type, Allocator>& rhs)
	{
		return simstd::rel_ops::operator <=(lhs, rhs);
	}

	template<typename Type, typename Allocator>
	bool operator >=(const vector<Type, Allocator>& lhs, const vector<Type, Allocator>& rhs)
	{
		return simstd::rel_ops::operator >=(lhs, rhs);
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
		TraceFunc();
	}

	template<typename Type, typename Allocator>
	vector<Type, Allocator>::vector()
		: vector(allocator_type())
	{
	}

	template<typename Type, typename Allocator>
	vector<Type, Allocator>::vector(const allocator_type& allocator)
		: base_type(allocator)
	{
		TraceFunc();
	}

	template<typename Type, typename Allocator>
	vector<Type, Allocator>::vector(size_type count, const value_type& value, const allocator_type& allocator)
		: base_type(count, allocator)
	{
		TraceFunc();
		impl.end = simstd::uninitialized_fill_n_a(impl.end, count, value, get_base_allocator());
	}

	template<typename Type, typename Allocator>
	vector<Type, Allocator>::vector(size_type count, const allocator_type& allocator)
		: base_type(count, allocator)
	{
		TraceFunc();
		impl.end = simstd::uninitialized_default_n_a(impl.end, count, get_base_allocator());
	}

	template<typename Type, typename Allocator>
	template<typename InputIterator>
	vector<Type, Allocator>::vector(InputIterator first, InputIterator last, const allocator_type& allocator)
		: base_type(allocator)
	{
		TraceFunc();
		_insert_back(first, last, simstd::pvt::iterator_category(first));
	}

	template<typename Type, typename Allocator>
	vector<Type, Allocator>::vector(const this_type& other)
		: vector(other, other.get_allocator())
	{
	}

	template<typename Type, typename Allocator>
	vector<Type, Allocator>::vector(const this_type& other, const allocator_type& allocator)
		: base_type(other.size(), allocator)
	{
		TraceFunc();
		impl.end = simstd::uninitialized_copy_a(other.impl.begin, other.impl.end, impl.end, get_base_allocator());
	}

	template<typename Type, typename Allocator>
	vector<Type, Allocator>::vector(this_type&& other)
		: base_type(simstd::move(other))
	{
		TraceFunc();
	}

	template<typename Type, typename Allocator>
	vector<Type, Allocator>::vector(this_type&& other, const allocator_type& allocator)
		: base_type(simstd::move(other), allocator)
	{
		TraceFunc();
		if (other.get_allocator() != allocator)
		{
			impl.end = simstd::uninitialized_copy_a(simstd::make_move_iterator(other.impl.begin), simstd::make_move_iterator(other.impl.end), impl.end, get_base_allocator());
			other.clear();
		}
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::this_type& vector<Type, Allocator>::operator =(const this_type& other)
	{
		TraceFunc();
		if (this != &other)
			this_type(other).swap(*this);
		return *this;
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::this_type& vector<Type, Allocator>::operator =(this_type&& other)
	{
		TraceFunc();
		if (this != &other)
			this_type(simstd::move(other)).swap(*this);
		return *this;
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::assign(size_type n, const value_type& value)
	{
		TraceFunc();
		this_type(n, value).swap(*this);
	}

	template<typename Type, typename Allocator>
	template<typename InputIterator>
	void vector<Type, Allocator>::assign(InputIterator first, InputIterator last)
	{
		TraceFunc();
		this_type(first, last).swap(*this);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::reference vector<Type, Allocator>::operator [](size_type pos)
	{
		return *(impl.begin + pos);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_reference vector<Type, Allocator>::operator [](size_type pos) const
	{
		return *(impl.begin + pos);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::reference vector<Type, Allocator>::at(size_type pos)
	{
		return *(impl.begin + pos);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_reference vector<Type, Allocator>::at(size_type pos) const
	{
		return *(impl.begin + pos);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::reference vector<Type, Allocator>::front()
	{
		return *(impl.begin);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_reference vector<Type, Allocator>::front() const
	{
		return *(impl.begin);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::reference vector<Type, Allocator>::back()
	{
		return *(impl.end - 1);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_reference vector<Type, Allocator>::back() const
	{
		return *(impl.end - 1);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::value_type* vector<Type, Allocator>::data()
	{
		return impl.begin;
	}

	template<typename Type, typename Allocator>
	const typename vector<Type, Allocator>::value_type* vector<Type, Allocator>::data() const
	{
		return impl.begin;
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::begin()
	{
		return iterator(impl.begin);
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
		return const_iterator(impl.begin);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::end()
	{
		return iterator(impl.end);
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
		return const_iterator(impl.end);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::reverse_iterator vector<Type, Allocator>::rbegin()
	{
		return reverse_iterator(iterator(impl.end));
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
		return const_reverse_iterator(const_iterator(impl.end));
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::reverse_iterator vector<Type, Allocator>::rend()
	{
		return reverse_iterator(iterator(impl.begin));
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
		return const_reverse_iterator(const_iterator(impl.begin));
	}

	template<typename Type, typename Allocator>
	bool vector<Type, Allocator>::empty() const
	{
		return impl.end == impl.begin;
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::size_type vector<Type, Allocator>::size() const
	{
		return base_type::size();
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::size_type vector<Type, Allocator>::max_size() const
	{
		return base_type::max_size();
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::reserve(size_type new_capa)
	{
		TraceFunc();
		_reserve(new_capa);
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::_reserve(size_type new_capa)
	{
		if (capacity() < new_capa)
			base_type::adjust_capacity(new_capa - size());
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::size_type vector<Type, Allocator>::capacity() const
	{
		return base_type::capacity();
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::shrink_to_fit()
	{
		TraceFunc();
		this_type(*this).swap(*this);
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::clear()
	{
		TraceFunc();
		base_type::destroy_till_end(impl.begin);
	}

	template<typename Type, typename Allocator>
	template<typename InputIterator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::insert(const_iterator cpos, InputIterator first, InputIterator last)
	{
		TraceFunc();
		return _insert(cpos, first, last, simstd::pvt::iterator_category(first));
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::insert(const_iterator cpos, const value_type& value)
	{
		TraceFunc();
		return _emplace(cpos, value);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::insert(const_iterator cpos, size_type n, const value_type& value)
	{
		TraceFunc();
		return _insert(cpos, n, value);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::insert(const_iterator cpos, value_type&& value)
	{
		TraceFunc();
		return _emplace(cpos, simstd::move(value));
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::erase(const_iterator cpos)
	{
		TraceFunc();
		return _erase(cpos, cpos + 1);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::erase(const_iterator cfirst, const_iterator clast)
	{
		TraceFunc();
		return _erase(cfirst, clast);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::_erase(const_iterator cfirst, const_iterator clast)
	{
		auto first = simstd::next(impl.begin, simstd::distance(cbegin(), cfirst));
		auto last = simstd::next(impl.begin, simstd::distance(cbegin(), clast));
		auto newEnd = simstd::move(last, impl.end, first);
		base_type::destroy_till_end(newEnd);
		return iterator(first);
	}

	template<typename Type, typename Allocator>
	template<typename... Args>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::emplace(const_iterator cpos, Args&&... args)
	{
		TraceFunc();
		return _emplace(cpos, simstd::forward<Args>(args)...);
	}

	template<typename Type, typename Allocator>
	template<typename... Args>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::_emplace(const_iterator cpos, Args&&... args)
	{
		auto distance = simstd::distance(cbegin(), cpos);
		iterator pos = simstd::next(begin(), distance);
		if (size() < capacity()) {
			if (cpos == cend()) {
				_emplace_back(simstd::forward<Args>(args)...);
			} else {
				get_base_allocator().construct(impl.end, simstd::move(*(impl.end - 1)));
				++impl.end;
				simstd::move_backward(pos.base(), impl.end - 2, impl.end - 1);
				*pos = value_type(simstd::forward<Args>(args)...);
			}
		} else {
			base_type newBase(base_type::get_new_capacity(1), get_base_allocator());
			newBase.impl.end = simstd::uninitialized_copy_a(simstd::make_move_iterator(impl.begin), simstd::make_move_iterator(pos.base()), newBase.impl.end, newBase.get_base_allocator());
			newBase.get_base_allocator().construct(newBase.impl.end++, simstd::forward<Args>(args)...);
			newBase.impl.end = simstd::uninitialized_copy_a(simstd::make_move_iterator(pos.base()), simstd::make_move_iterator(impl.end), newBase.impl.end, newBase.get_base_allocator());
			newBase.swap(*this);
		}
		return iterator(impl.begin + distance);
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::push_back(const value_type& value)
	{
		TraceFunc();
		_emplace_back(value);
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::push_back(value_type&& value)
	{
		TraceFunc();
		_emplace_back(simstd::move(value));
	}

	template<typename Type, typename Allocator>
	template<typename... Args>
	void vector<Type, Allocator>::emplace_back(Args&&... args)
	{
		TraceFunc();
		_emplace_back(simstd::forward<Args>(args)...);
	}

	template<typename Type, typename Allocator>
	template<typename... Args>
	void vector<Type, Allocator>::_emplace_back(Args&&... args)
	{
		base_type::adjust_capacity(1);
		get_base_allocator().construct(impl.end++, simstd::forward<Args>(args)...);
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::pop_back()
	{
		TraceFunc();
		_erase(cend() - 1, cend());
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::resize(size_type count)
	{
		TraceFunc();
		if (size() < count) {
			_reserve(count);
			impl.end = simstd::uninitialized_default_n_a(impl.end, count - size(), get_base_allocator());
		} else if (count < size()) {
			base_type::destroy_till_end(impl.begin + count);
		}
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::resize(size_type count, const value_type& value)
	{
		TraceFunc();
		if (size() < count) {
			_reserve(count);
			impl.end = simstd::uninitialized_fill_n_a(impl.end, count - size(), value, get_base_allocator());
		} else if (count < size()) {
			base_type::destroy_till_end(impl.begin + count);
		}
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::swap(this_type& other)
	{
		TraceFunc();
		base_type::swap(other);
	}

	template<typename Type, typename Allocator>
	template<typename InputIterator>
	void vector<Type, Allocator>::_insert_back(InputIterator first, InputIterator last, simstd::input_iterator_tag)
	{
		simstd::copy(first, last, simstd::back_inserter(*this));
	}

	template<typename Type, typename Allocator>
	template<typename ForwardIterator>
	void vector<Type, Allocator>::_insert_back(ForwardIterator first, ForwardIterator last, simstd::forward_iterator_tag)
	{
		base_type::adjust_capacity(simstd::distance(first, last));
		impl.end = simstd::uninitialized_copy_a(first, last, impl.end, get_base_allocator());
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::_insert(const_iterator cpos, size_type n, const value_type& value)
	{
		simstd::pvt::_value_generator<Type> generator(n, value);
		return _insert(cpos, generator.begin(), generator.end(), simstd::pvt::iterator_category(generator.begin()));
	}

	template<typename Type, typename Allocator>
	template<typename InputIterator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::_insert(const_iterator cpos, InputIterator first, InputIterator last, simstd::input_iterator_tag)
	{
		auto distance = simstd::distance(cbegin(), cpos);
		simstd::copy(first, last, simstd::inserter(*this, cpos));
		return begin() + distance;
	}

	template<typename Type, typename Allocator>
	template<typename ForwardIterator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::_insert(const_iterator cpos, ForwardIterator first, ForwardIterator last, simstd::forward_iterator_tag)
	{
		auto distance = simstd::distance(cbegin(), cpos);
		auto n = simstd::distance(first, last);
		auto pos = simstd::next(begin(), distance);
		if (size() + n <= capacity()) {
			auto elemsToMove = end() - pos;
			auto old_end = impl.end;
			if (elemsToMove < n) {
				auto mid = simstd::next(first, elemsToMove);
				impl.end = simstd::uninitialized_copy_a(mid, last, impl.end, get_base_allocator());
				impl.end = simstd::uninitialized_copy_a(simstd::make_move_iterator(pos.base()), simstd::make_move_iterator(old_end), impl.end, get_base_allocator());
				simstd::copy(first, mid, pos.base());
			} else {
				impl.end = simstd::uninitialized_copy_a(simstd::make_move_iterator(impl.end - n), simstd::make_move_iterator(impl.end), impl.end, get_base_allocator());
				simstd::move_backward(pos.base(), old_end - n, old_end);
				simstd::copy(first, last, pos.base());
			}
		} else {
			base_type newBase(base_type::get_new_capacity(n), get_base_allocator());
			newBase.impl.end = simstd::uninitialized_copy_a(simstd::make_move_iterator(impl.begin), simstd::make_move_iterator(pos.base()), newBase.impl.end, newBase.get_base_allocator());
			newBase.impl.end = simstd::uninitialized_copy_a(first, last, newBase.impl.end, get_base_allocator());
			newBase.impl.end = simstd::uninitialized_copy_a(simstd::make_move_iterator(pos.base()), simstd::make_move_iterator(impl.end), newBase.impl.end, newBase.get_base_allocator());
			newBase.swap(*this);
		}
		return iterator(impl.begin + distance);
	}

}

#endif
