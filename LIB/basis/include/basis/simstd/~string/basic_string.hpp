#ifndef BASIS_SIMSTD_STRING_BASIC_STRING_HPP_
#define BASIS_SIMSTD_STRING_BASIC_STRING_HPP_

template<typename CharType, typename Traits, typename Allocator>
class simstd::basic_string: private pvt::basic_string_base<CharType, Allocator> {
	using this_type = basic_string;
	using base_type = pvt::basic_string_base<CharType, Allocator>;
	using base_type::impl;

public:
	using traits_type = Traits;
	using size_type = size_t;
	using difference_type = ptrdiff_t;

	using value_type = typename traits_type::char_type;
	using allocator_type = Allocator;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;

	using iterator = simstd::pvt::normal_iterator<pointer>;
	using const_iterator = simstd::pvt::normal_iterator<const_pointer>;
	using reverse_iterator = simstd::reverse_iterator<iterator>;
	using const_reverse_iterator = simstd::reverse_iterator<const_iterator>;

	static const size_type npos = ~static_cast<size_type>(0);

	~basic_string() noexcept;

	basic_string() noexcept;
	explicit basic_string(const allocator_type& allocator);

	basic_string(size_type count, value_type ch, const allocator_type& allocator = allocator_type());
	basic_string(const this_type& other, size_type index, size_type count = npos, const allocator_type& allocator = allocator_type());
	basic_string(const_pointer str, size_type len, const allocator_type& allocator = allocator_type());
	basic_string(const_pointer str, const allocator_type& allocator = allocator_type());
	template<typename Iterator>
	basic_string(Iterator first, Iterator last, const allocator_type& allocator = allocator_type());
	basic_string(const this_type& other);
	basic_string(const this_type& other, const allocator_type& allocator);
	basic_string(this_type&& other) noexcept;
	basic_string(this_type&& other, const allocator_type& allocator) noexcept;
//	basic_string(std::initializer_list<value_type> ilist, const allocator_type& allocator = allocator_type());

	this_type& operator =(const this_type& other);
	this_type& operator =(this_type&& other);
	this_type& operator =(const_pointer other);
	this_type& operator =(value_type ch);

	this_type& assign(size_type count, value_type ch);
	this_type& assign(const this_type& other);
	this_type& assign(const this_type& other, size_type index, size_type count = npos);
	this_type& assign(this_type&& other) noexcept;
	this_type& assign(const_pointer str, size_type len);
	this_type& assign(const_pointer str);
	template<typename Iterator>
	this_type& assign(Iterator first, Iterator last);

	allocator_type get_allocator() const;

	// Element access
	const_reference at(size_type in) const;
	reference       at(size_type in);

	const_reference operator [](size_type in) const;
	reference       operator [](size_type in);

	const_reference front() const;
	reference       front();

	const_reference back() const;
	reference       back();

	const_pointer   data() const;

	const_pointer   c_str() const;

	// Iterators
	const_iterator cbegin() const;
	const_iterator begin() const;
	iterator begin();

	const_iterator cend() const;
	const_iterator end() const;
	iterator end();

	const_iterator rcbegin() const;
	const_iterator rbegin() const;
	iterator rbegin();

	const_iterator rcend() const;
	const_iterator rend() const;
	iterator rend();

	// Capacity
	bool empty() const;

	size_type size() const;
	size_type length() const;

	size_type max_size() const;

	void reserve(size_type capa);

	size_type capacity() const;

	void shrink_to_fit();

	// Operations
	void clear();

	this_type&  insert(size_type index, size_type count, value_type ch);
	this_type&  insert(size_type index, const_pointer str);
	this_type&  insert(size_type index, const_pointer str, size_type len);
	this_type&  insert(size_type index, const this_type& str);
	this_type&  insert(size_type index, const this_type& str, size_type pos, size_type len = npos);
	iterator    insert(const_iterator cpos, value_type ch);
	iterator    insert(const_iterator cpos, size_type count, value_type ch);
	template<typename Iterator>
	iterator    insert(const_iterator cpos, Iterator first, Iterator last);

	this_type&  erase(size_type index = 0, size_type count = npos);
	iterator    erase(const_iterator cpos);
	iterator    erase(const_iterator first, const_iterator last);

	void push_back(value_type ch);

	void pop_back() noexcept;

	this_type& append(size_type count, value_type ch);
	this_type& append(const this_type& str);
	this_type& append(const this_type& str, size_type index, size_type count = npos);
	this_type& append(const_pointer str, size_type len);
	this_type& append(const_pointer str);
	template<typename Iterator>
	this_type& append(Iterator first, Iterator last);

	this_type& operator +=(const this_type& str);
	this_type& operator +=(const_pointer str);
	this_type& operator +=(value_type ch);

	int compare(const_pointer str) const;
	int compare(const this_type& str) const;
	int compare(size_type index, size_type count, const this_type& str) const;
	int compare(size_type index, size_type count, const_pointer str) const;
	int compare(size_type index, size_type count, const_pointer str, size_type len) const;

	this_type& replace(size_type index, size_type count, const this_type& other);
	this_type& replace(const_iterator first, const_iterator last, const this_type& other);
	this_type& replace(size_type index, size_type count, const this_type& str, size_type subpos, size_type sublen = npos);
	template<typename Iterator>
	this_type& replace(const_iterator first, const_iterator last, Iterator first2, Iterator last2);
	this_type& replace(size_type index, size_type count, const_pointer str, size_type len);
	this_type& replace(const_iterator first, const_iterator last, const_pointer str, size_type count);
	this_type& replace(size_type index, size_type count, const_pointer str);
	this_type& replace(const_iterator first, const_iterator last, const_pointer str);
	this_type& replace(size_type index, size_type count, size_type len, value_type ch);
	this_type& replace(const_iterator first, const_iterator last, size_type count, value_type ch);

	this_type substr(size_type index = 0, size_type count = npos) const;

	size_type copy(pointer dest, size_type count, size_type index = 0) const;

	void resize(size_type count);
	void resize(size_type count, value_type ch);

	void swap(this_type& other) noexcept;

	// Search
	size_type find(const this_type& str, size_type index = 0) const;
	size_type find(const_pointer str, size_type index = 0) const;
	size_type find(value_type ch, size_type index = 0) const;

	size_type rfind(const this_type& str, size_type index = npos) const;
	size_type rfind(const_pointer str, size_type index, size_type count) const;
	size_type rfind(const_pointer str, size_type index = npos) const;
	size_type rfind(value_type ch, size_type index = npos) const;

	size_type find_first_of(const this_type& str, size_type index = 0) const;
	size_type find_first_not_of(const this_type& str, size_type index = 0) const;
	size_type find_last_of(const this_type& str, size_type index = npos) const;
	size_type find_last_not_of(const this_type& str, size_type index = npos) const;

private:
	basic_string(const Allocator& allocator, size_type capacity, const_pointer str1, size_type len1, const_pointer str2 = nullptr, size_type len2 = 0);

	template<typename InputIt>
	basic_string(InputIt first, InputIt last, const allocator_type& allocator, simstd::input_iterator_tag);
	template<typename ForwardIt>
	basic_string(ForwardIt first, ForwardIt last, const allocator_type& allocator, simstd::forward_iterator_tag);

	void _raw_append(value_type ch, size_type count);
	void _raw_append(const_pointer str, size_type count);
	template<typename InputIt>
	void _raw_append(InputIt first, InputIt last);

	iterator _erase(size_type index, size_type count);

	template<typename InputIt>
	this_type& _append(InputIt first, InputIt last, simstd::input_iterator_tag);
	template<typename ForwardIt>
	this_type& _append(ForwardIt first, ForwardIt last, simstd::forward_iterator_tag);

	template<typename InputIt>
	this_type& _assign(InputIt first, InputIt last, simstd::input_iterator_tag);
	template<typename ForwardIt>
	this_type& _assign(ForwardIt first, ForwardIt last, simstd::forward_iterator_tag);

	template<typename Iterator>
	this_type& _replace(const_iterator first, const_iterator last, Iterator first2, Iterator last2, simstd::input_iterator_tag);
	template<typename Iterator>
	this_type& _replace(const_iterator first, const_iterator last, Iterator first2, Iterator last2, simstd::forward_iterator_tag);

	size_type get_necessary_capacity(ptrdiff_t addToSize) const;

	bool must_be_splitted(size_type minCapacity);
	void split_and_clear(size_type minCapacity);
	void split_and_copy(size_type minCapacity);

	static int _compare(const_pointer str1, size_type len1, const_pointer str2, size_type len2);

	size_type find(const_pointer str, size_type index, size_type length) const;

	bool is_same_str(const_pointer str) const;

	template<typename C, typename T, typename A> friend basic_string<C, T, A> operator +(const basic_string<C, T, A>& a, const basic_string<C, T, A>& b);
	template<typename C, typename T, typename A> friend basic_string<C, T, A> operator +(const C* a, const basic_string<C, T, A>& b);
	template<typename C, typename T, typename A> friend basic_string<C, T, A> operator +(C a, const basic_string<C, T, A>& b);
	template<typename C, typename T, typename A> friend basic_string<C, T, A> operator +(const basic_string<C, T, A>& a, const C* b);
	template<typename C, typename T, typename A> friend basic_string<C, T, A> operator +(const basic_string<C, T, A>& a, C b);

	template<typename C, typename T, typename A> friend bool operator ==(const basic_string<C, T, A>& a, const basic_string<C, T, A>& b);
	template<typename C, typename T, typename A> friend bool operator ==(const C* a, const basic_string<C, T, A>& b);
	template<typename C, typename T, typename A> friend bool operator ==(const basic_string<C, T, A>& a, const C* b);
	template<typename C, typename T, typename A> friend bool operator !=(const basic_string<C, T, A>& a, const basic_string<C, T, A>& b);
	template<typename C, typename T, typename A> friend bool operator !=(const C* a, const basic_string<C, T, A>& b);
	template<typename C, typename T, typename A> friend bool operator !=(const basic_string<C, T, A>& a, const C* b);
	template<typename C, typename T, typename A> friend bool operator <(const basic_string<C, T, A>& a, const basic_string<C, T, A>& b);
	template<typename C, typename T, typename A> friend bool operator <(const C* a, const basic_string<C, T, A>& b);
	template<typename C, typename T, typename A> friend bool operator <(const basic_string<C, T, A>& a, const C* b);
	template<typename C, typename T, typename A> friend bool operator <=(const basic_string<C, T, A>& a, const basic_string<C, T, A>& b);
	template<typename C, typename T, typename A> friend bool operator <=(const C* a, const basic_string<C, T, A>& b);
	template<typename C, typename T, typename A> friend bool operator <=(const basic_string<C, T, A>& a, const C* b);
	template<typename C, typename T, typename A> friend bool operator >(const basic_string<C, T, A>& a, const basic_string<C, T, A>& b);
	template<typename C, typename T, typename A> friend bool operator >(const C* a, const basic_string<C, T, A>& b);
	template<typename C, typename T, typename A> friend bool operator >(const basic_string<C, T, A>& a, const C* b);
	template<typename C, typename T, typename A> friend bool operator >=(const basic_string<C, T, A>& a, const basic_string<C, T, A>& b);
	template<typename C, typename T, typename A> friend bool operator >=(const C* a, const basic_string<C, T, A>& b);
	template<typename C, typename T, typename A> friend bool operator >=(const basic_string<C, T, A>& a, const C* b);
};

namespace simstd
{
	template<typename C, typename T, typename A>
	basic_string<C, T, A>::~basic_string() noexcept
	{
		TraceObj();
	}

	template<typename C, typename T, typename A>
	basic_string<C, T, A>::basic_string() noexcept :
		base_type(allocator_type())
	{
		TraceObj();
	}

	template<typename C, typename T, typename A>
	basic_string<C, T, A>::basic_string(const allocator_type& allocator):
		base_type(allocator)
	{
		TraceObj();
	}

	template<typename C, typename T, typename A>
	basic_string<C, T, A>::basic_string(size_type count, value_type ch, const allocator_type& allocator):
		base_type(allocator, count)
	{
		TraceObj();
		_raw_append(ch, count);
	}

	template<typename C, typename T, typename A>
	basic_string<C, T, A>::basic_string(const this_type& other, size_type index, size_type count, const allocator_type& allocator):
		base_type(allocator, (count == npos) ? count = other.size() - index : count)
	{
		TraceObj();
		_raw_append(other.c_str() + index, count);
	}

	template<typename C, typename T, typename A>
	basic_string<C, T, A>::basic_string(const_pointer str, size_type count, const allocator_type& allocator):
		this_type(allocator, count, str, count)
	{
	}

	template<typename C, typename T, typename A>
	basic_string<C, T, A>::basic_string(const_pointer str, const allocator_type& allocator):
		this_type(str, str ? traits_type::length(str) : 0, allocator)
	{
	}

	template<typename C, typename T, typename A>
	template<typename Iterator>
	basic_string<C, T, A>::basic_string(Iterator first, Iterator last, const allocator_type& allocator):
		this_type(first, last, allocator, simstd::pvt::iterator_category(first))
	{
		TraceObj();
	}

	template<typename C, typename T, typename A>
	basic_string<C, T, A>::basic_string(const this_type& other):
		base_type(other)
	{
		TraceObj();
	}

	template<typename C, typename T, typename A>
	basic_string<C, T, A>::basic_string(const this_type& other, const allocator_type& allocator):
		base_type(other, allocator)
	{
		TraceObj();
		if (impl != other.impl)
			_raw_append(other.c_str(), other.size());
	}

	template<typename C, typename T, typename A>
	basic_string<C, T, A>::basic_string(this_type&& other):
		base_type(simstd::move(other))
	{
		TraceObj();
	}

	template<typename C, typename T, typename A>
	basic_string<C, T, A>::basic_string(this_type&& other, const allocator_type& allocator):
		base_type(simstd::move(other), allocator)
	{
		TraceObj();
		if (impl != other.impl)
			_raw_append(other.c_str(), other.size());
	}

//	template<typename C, typename T, typename A>
//	basic_string<C, T, A>::basic_string(std::initializer_list<value_type> ilist) :
//		m_data(string_impl::allocate(get_new_capacity(ilist.size())))
//	{
//		m_data->append(ilist.begin(), ilist.end());
//	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::operator =(const this_type& other)
	{
		return assign(other);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::operator =(this_type&& other) noexcept
	{
		return assign(simstd::move(other));
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::operator =(const_pointer other)
	{
		return assign(other);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::operator =(value_type ch)
	{
		return assign(1, ch);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::assign(size_type count, value_type ch)
	{
		TraceObj();
		split_and_clear(count);
		_raw_append(ch, count);
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::assign(const this_type& other)
	{
		TraceObj();
		if (impl != other.impl) {
			this_type(other).swap(*this);
		}
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::assign(const this_type& other, size_type index, size_type count)
	{
		TraceObj();
		CRT_ASSERT(index <= other.size());
		split_and_clear(count == npos ? count = other.size() - index : count);
		_raw_append(other.c_str() + index, count);
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::assign(this_type&& other) noexcept
	{
		TraceObj();
		CRT_ASSERT(impl != other.impl); // UB
		swap(other);
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::assign(const_pointer str, size_type len)
	{
		TraceFormatObj("(%p, %Iu)\n", str, len);
		if (is_same_str(str)) {
			this_type(get_allocator(), len, str, len).swap(*this);
		} else {
			split_and_clear(len);
			_raw_append(str, len);
		}
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::assign(const_pointer str)
	{
		return assign(str, traits_type::length(str));
	}

	template<typename C, typename T, typename A>
	template<typename Iterator>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::assign(Iterator first, Iterator last)
	{
		return _assign(first, last, simstd::pvt::iterator_category(first));
	}

	template<typename C, typename T, typename A>
	template<typename InputIt>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::_assign(InputIt first, InputIt last, simstd::input_iterator_tag tag)
	{
		split_and_clear(capacity());
		_append(first, last, tag);
		return *this;
	}

	template<typename C, typename T, typename A>
	template<typename ForwardIt>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::_assign(ForwardIt first, ForwardIt last, simstd::forward_iterator_tag)
	{
		split_and_clear(simstd::distance(first, last));
		_raw_append(first, last);
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::allocator_type basic_string<C, T, A>::get_allocator() const
	{
		return impl->get_allocator();
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::const_reference basic_string<C, T, A>::at(size_type in) const
	{
		return *(impl->get_data() + in);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::reference basic_string<C, T, A>::at(size_type in)
	{
		CRT_ASSERT(in < length());
		split_and_copy(capacity());
		return *(impl->get_data() + in);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::const_reference basic_string<C, T, A>::operator [](size_type in) const
	{
		return *(impl->get_data() + in);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::reference basic_string<C, T, A>::operator [](size_type in)
	{
		split_and_copy(capacity());
		return *(impl->get_data() + in);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::const_reference basic_string<C, T, A>::front() const
	{
		CRT_ASSERT(!empty());
		return *impl->get_data();
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::reference basic_string<C, T, A>::front()
	{
		CRT_ASSERT(!empty());
		split_and_copy(capacity());
		return *impl->get_data();
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::const_reference basic_string<C, T, A>::back() const
	{
		CRT_ASSERT(!empty());
		return *(impl->get_data() + size() - 1);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::reference basic_string<C, T, A>::back()
	{
		CRT_ASSERT(!empty());
		split_and_copy(capacity());
		return *(impl->get_data() + size() - 1);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::const_pointer basic_string<C, T, A>::data() const
	{
		return impl->get_data();
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::const_pointer basic_string<C, T, A>::c_str() const
	{
		return impl->get_data();
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::const_iterator basic_string<C, T, A>::cbegin() const
	{
		return const_iterator(c_str());
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::const_iterator basic_string<C, T, A>::begin() const
	{
		return cbegin();
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::iterator basic_string<C, T, A>::begin()
	{
		return iterator(impl->get_data());
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::const_iterator basic_string<C, T, A>::cend() const
	{
		return cbegin() + size();
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::const_iterator basic_string<C, T, A>::end() const
	{
		return cbegin() + size();
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::iterator basic_string<C, T, A>::end()
	{
		return begin() + size();
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::const_iterator basic_string<C, T, A>::rcbegin() const
	{
		return const_reverse_iterator(cend());
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::const_iterator basic_string<C, T, A>::rbegin() const
	{
		return rcbegin();
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::iterator basic_string<C, T, A>::rbegin()
	{
		return reverse_iterator(end());
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::const_iterator basic_string<C, T, A>::rcend() const
	{
		return const_reverse_iterator(cbegin());
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::const_iterator basic_string<C, T, A>::rend() const
	{
		return rcend();
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::iterator basic_string<C, T, A>::rend()
	{
		return reverse_iterator(begin());
	}

	template<typename C, typename T, typename A>
	bool basic_string<C, T, A>::empty() const
	{
		return impl->get_size() == 0;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::size() const
	{
		return impl->get_size();
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::length() const
	{
		return impl->get_size();
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::max_size() const
	{
		return npos;
	}

	template<typename C, typename T, typename A>
	void basic_string<C, T, A>::reserve(size_type capa)
	{
		TraceObj();
		split_and_copy(capa);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::capacity() const
	{
		return impl->get_capa();
	}

	template<typename C, typename T, typename A>
	void basic_string<C, T, A>::shrink_to_fit()
	{
		TraceObj();
		if (size() < capacity())
			this_type(get_allocator(), size(), c_str(), size()).swap(*this);
	}

	template<typename C, typename T, typename A>
	void basic_string<C, T, A>::clear()
	{
		TraceObj();
		if (!empty())
			split_and_clear(capacity());
	}

//	template<typename C, typename T, typename A>
//	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::insert(size_type index, size_type count, value_type ch)
//	{
//		return replace(index, 0, count, ch);
//	}
//
//	template<typename C, typename T, typename A>
//	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::insert(size_type index, const_pointer str)
//	{
//		return replace(index, 0, str, traits_type::length(str));
//	}
//
//	template<typename C, typename T, typename A>
//	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::insert(size_type index, const_pointer str, size_type count)
//	{
//		return replace(index, 0, str, count);
//	}
//
//	template<typename C, typename T, typename A>
//	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::insert(size_type index, const this_type& str)
//	{
//		return replace(index, 0, str.c_str(), str.length());
//	}
//
//	template<typename C, typename T, typename A>
//	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::insert(size_type index, const this_type& str, size_type index_str, size_type count)
//	{
//		return replace(index, 0, str.c_str(), count);
//	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::iterator basic_string<C, T, A>::insert(const_iterator cpos, value_type ch)
	{
		TraceObj();
		auto insertPos = simstd::distance(cbegin(), cpos);
		insert(insertPos, 0, 1, ch);
		return iterator(impl->get_data() + insertPos);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::iterator basic_string<C, T, A>::insert(const_iterator cpos, size_type count, value_type ch)
	{
		TraceObj();
		auto insertPos = simstd::distance(cbegin(), cpos);
		insert(insertPos, 0, count, ch);
		return iterator(impl->get_data() + insertPos);
	}

	template<typename C, typename T, typename A>
	template<typename Iterator>
	typename basic_string<C, T, A>::iterator basic_string<C, T, A>::insert(const_iterator cpos, Iterator first, Iterator last)
	{
		TraceObj();
		auto insertPos = simstd::distance(cbegin(), cpos);
		CRT_ASSERT(false); // TODO not implemented
		return iterator(impl->get_data() + insertPos);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::erase(size_type index, size_type count)
	{
		TraceFormatObj("(%Iu, %Iu)\n", index, count);
		CRT_ASSERT(index <= size());
		_erase(index, (count == npos) ? size() - index : count);
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::iterator basic_string<C, T, A>::erase(const_iterator cpos)
	{
		TraceFormatObj("(%p), [%p]\n", &*cpos, c_str());
		return _erase(simstd::distance(cbegin(), cpos), 1);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::iterator basic_string<C, T, A>::erase(const_iterator first, const_iterator last)
	{
		TraceFormatObj("(%p, %p), [%p]\n", &*first, &*last, c_str());
		return _erase(simstd::distance(cbegin(), first), simstd::distance(first, last));
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::iterator basic_string<C, T, A>::_erase(size_type index, size_type count)
	{
		auto indexLast = index + count;
		CRT_ASSERT(index <= size());
		CRT_ASSERT(indexLast <= size());
		if (impl->is_shared()) {
			this_type(get_allocator(), capacity(), c_str(), index, c_str() + indexLast, size() - indexLast).swap(*this);
		} else {
			traits_type::move(impl->get_data() + index, impl->get_data() + indexLast, size() - indexLast);
			impl->set_size(size() - count);
		}
		return iterator(impl->get_data() + index);
	}

	template<typename C, typename T, typename A>
	void basic_string<C, T, A>::push_back(value_type ch)
	{
		TraceFormatObj("('%c')\n", ch);
		split_and_copy(get_necessary_capacity(1));
		_raw_append(ch, 1);
	}

	template<typename C, typename T, typename A>
	void basic_string<C, T, A>::pop_back() noexcept
	{
		TraceObj();
		CRT_ASSERT(!empty());
		split_and_copy(capacity());
		impl->set_size(size() - 1);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::append(size_type count, value_type ch)
	{
		TraceFormatObj("(%Iu, '%c')\n", count, ch);
		split_and_copy(get_necessary_capacity(count));
		_raw_append(ch, count);
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::append(const this_type& other)
	{
		TraceObj();
		return append(other.c_str(), other.size());
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::append(const this_type& other, size_type index, size_type count)
	{
		TraceObj();
		return append(other.c_str() + index, count == npos ? other.size() - index : count);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::append(const_pointer str, size_type len)
	{
		TraceObj();
		CRT_ASSERT(str);
		if (len) {
			auto capa = get_necessary_capacity(len);
			if (is_same_str(str)) {
				this_type(get_allocator(), capa, c_str(), size(), str, len).swap(*this);
			} else {
				split_and_copy(capa);
				_raw_append(str, len);
			}
		}
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::append(const_pointer str)
	{
		TraceObj();
		return append(str, traits_type::length(str));
	}

	template<typename C, typename T, typename A>
	template<typename Iterator>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::append(Iterator first, Iterator last)
	{
		TraceObj();
		return _append(first, last, simstd::pvt::iterator_category(first));
	}

	template<typename C, typename T, typename A>
	template<typename InputIt>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::_append(InputIt first, InputIt last, simstd::input_iterator_tag)
	{
		for (; first != last; ++first) {
			split_and_copy(get_necessary_capacity(1));
			_raw_append(1, *first);
		}
		return *this;
	}

	template<typename C, typename T, typename A>
	template<typename ForwardIt>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::_append(ForwardIt first, ForwardIt last, simstd::forward_iterator_tag)
	{
		split_and_copy(get_necessary_capacity(simstd::distance(first, last)));
		_raw_append(first, last);
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::operator +=(const this_type& other)
	{
		TraceObj();
		return append(other);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::operator +=(const_pointer str)
	{
		TraceObj();
		return append(str);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::operator +=(value_type ch)
	{
		TraceObj();
		return append(1, ch);
	}

//	template<typename C, typename T, typename A>
//	int basic_string<C, T, A>::compare(const_pointer str) const
//	{
//		return compare(c_str(), length(), str, traits_type::length(str));
//	}
//
//	template<typename C, typename T, typename A>
//	int basic_string<C, T, A>::compare(const this_type& str) const
//	{
//		return compare(c_str(), length(), str.c_str(), str.length());
//	}
//
//	template<typename C, typename T, typename A>
//	int basic_string<C, T, A>::compare(size_type index, size_type count, const this_type& str) const
//	{
//		return compare(c_str() + index, count, str.c_str(), str.length());
//	}
//
//	template<typename C, typename T, typename A>
//	int basic_string<C, T, A>::compare(size_type index, size_type count, const_pointer str) const
//	{
//		return compare(c_str() + index, count, str, traits_type::length(str));
//	}
//
//	template<typename C, typename T, typename A>
//	int basic_string<C, T, A>::compare(size_type index, size_type count, const_pointer str, size_type len) const
//	{
//		return compare(c_str() + index, count, str, len);
//	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::replace(size_type index, size_type len, const this_type& other)
	{
		TraceObj();
		return replace(index, len, other.c_str(), other.size());
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::replace(const_iterator first, const_iterator last, const this_type& other)
	{
		TraceObj();
		return replace(first, last, other.c_str(), other.size());
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::replace(size_type index, size_type count, const this_type& other, size_type pos, size_type len)
	{
		TraceObj();
		CRT_ASSERT(index <= length());
		CRT_ASSERT(pos <= other.length());
		return replace(index, count, other.c_str() + pos, len == npos ? other.size() - pos : len);
	}

	template<typename C, typename T, typename A>
	template<typename Iterator>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::replace(const_iterator first, const_iterator last, Iterator first2, Iterator last2)
	{
		TraceObj();
		return _replace(first, last, first2, last2, simstd::pvt::iterator_category(first2));
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::replace(size_type index, size_type count, const_pointer str, size_type len)
	{
		TraceFormatObj("(%Iu, %Iu, %p, %Iu)\n", index, count, str, len);
		CRT_ASSERT(str);
//		printf("      (%Iu, %Iu): '%s'\n", size(), capacity(), c_str());
		index = simstd::min(index, size());
		count = simstd::min(count, size() - index);
//		printf("      (index: %Iu, count: %Iu)\n", index, count);
		auto capa = get_necessary_capacity(len - count);
		if (must_be_splitted(capa) || is_same_str(str)) {
			this_type tmp(get_allocator(), capa, c_str(), index, str, len);
//			printf("      (%Iu, %Iu): '%s'\n", tmp.size(), tmp.capacity(), tmp.c_str());
			tmp._raw_append(c_str() + index, size() - index);
//			printf("      (%Iu, %Iu): '%s'\n", tmp.size(), tmp.capacity(), tmp.c_str());
			tmp.swap(*this);
		} else {
//			printf("      (%Iu, %Iu): '%s'\n", size(), capacity(), c_str());
//			printf("replace before: '%s'\n", c_str());
			traits_type::move(impl->get_data() + index + len, impl->get_data() + index + count, size() - (index + count));
//			traits_type::assign(impl->get_data() + index, len, ' ');
//			printf("replace after : '%s'\n", c_str());
			traits_type::copy(impl->get_data() + index, str, len);
//			printf("replace after : '%s'\n", c_str());
			impl->set_size(size() + len - count);
//			printf("      (%Iu, %Iu): '%s'\n", size(), capacity(), c_str());
		}
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::replace(const_iterator first, const_iterator last, const_pointer str, size_type len)
	{
		TraceObj();
		return replace(simstd::distance(cbegin(), first), simstd::distance(first, last), str, len);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::replace(size_type index, size_type len, const_pointer str)
	{
		TraceObj();
		return replace(index, len, str, traits_type::length(str));
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::replace(const_iterator first, const_iterator last, const_pointer str)
	{
		TraceObj();
		return replace(first, last, str, traits_type::length(str));
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::replace(size_type index, size_type count, size_type len, value_type ch)
	{
		TraceObj();
		TraceFormatObj("(%Iu, %Iu, '%c', %Iu)\n", index, count, ch, len);
		CRT_ASSERT(index <= length());
		index = simstd::min(index, size());
		count = simstd::min(count, size() - index);
		auto capa = get_necessary_capacity(len - count);
		if (must_be_splitted(capa)) {
			this_type tmp(get_allocator(), capa, c_str(), index);
			tmp._raw_append(ch, len);
			tmp._raw_append(c_str() + index, size() - index);
			tmp.swap(*this);
		} else {
			traits_type::move(impl->get_data() + index + len, impl->get_data() + index + count, size() - (index + count));
			traits_type::assign(impl->get_data() + index, len, ch);
			impl->set_size(size() + len - count);
		}
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::replace(const_iterator first, const_iterator last, size_type len, value_type ch)
	{
		TraceObj();
		return replace(simstd::distance(cbegin(), first), simstd::distance(first, last), len, ch);
	}

	template<typename C, typename T, typename A>
	template<typename Iterator>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::_replace(const_iterator first, const_iterator last, Iterator first2, Iterator last2, simstd::input_iterator_tag tag)
	{
		auto index = simstd::distance(cbegin(), first);
		auto count = simstd::distance(first, last);
		this_type tmp(get_allocator(), capacity(), c_str(), index);
		tmp._append(first2, last2, tag);
		tmp.append(c_str() + index + count, size() - (index + count));
		tmp.swap(*this);
		return *this;
	}

	template<typename C, typename T, typename A>
	template<typename Iterator>
	typename basic_string<C, T, A>::this_type& basic_string<C, T, A>::_replace(const_iterator first, const_iterator last, Iterator first2, Iterator last2, simstd::forward_iterator_tag)
	{
		auto index = simstd::distance(cbegin(), first);
		auto count = simstd::distance(first, last);
		auto len = simstd::distance(first2, last2);
		this_type tmp(get_allocator(), get_necessary_capacity(len - count), c_str(), index);
		tmp._raw_append(first2, last2);
		tmp._raw_append(c_str() + index + count, size() - (index + count));
		tmp.swap(*this);
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type basic_string<C, T, A>::substr(size_type index, size_type count) const
	{
		TraceObj();
		CRT_ASSERT(index <= size());
		count = simstd::min(count, size() - index);
		return this_type(get_allocator(), count, c_str() + index, count);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::copy(pointer dest, size_type count, size_type index) const
	{
		TraceObj();
		count = (count == npos) ? size() - index : count;
		traits_type::move(dest, c_str() + index, count);
		return count;
	}

	template<typename C, typename T, typename A>
	void basic_string<C, T, A>::resize(size_type count)
	{
		TraceObj();
		// TODO
	}

	template<typename C, typename T, typename A>
	void basic_string<C, T, A>::resize(size_type count, value_type ch)
	{
		TraceObj();
		// TODO
	}

	template<typename C, typename T, typename A>
	void basic_string<C, T, A>::swap(this_type& other) noexcept
	{
		TraceObj();
		using simstd::swap;
		swap(impl, other.impl);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::find(const this_type& str, size_type index) const
	{
		return find(str.c_str(), index, str.length());
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::find(const_pointer str, size_type index) const
	{
		return find(str, index, traits_type::length(str));
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::find(value_type ch, size_type index) const
	{
		const_pointer find = traits_type::find(c_str() + index, size(), ch);
		return (find) ? find - c_str() : npos;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::rfind(const this_type& str, size_type index) const
	{
		return rfind(str.c_str(), index, str.length());
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::rfind(const_pointer str, size_type index, size_type count) const
	{
		index = simstd::min(index, size());
		if (count != 0 && count <= index) {
			index -= (count - 1);
			while (index > 0)
				if (find(str, index, count) != npos)
					return index;
		}
		return npos;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::rfind(const_pointer str, size_type index) const
	{
		return rfind(str, index, traits_type::length(str));
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::rfind(value_type ch, size_type index) const
	{
		index = simstd::min(index, size());
		while (index > 0)
			if (traits_type::eq(at(--index), ch))
				return index;
		return npos;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::find_first_of(const this_type& str, size_type index) const
	{
		for (; index < size(); ++index)
			if (traits_type::find(str.c_str(), str.size(), *(c_str() + index)) != nullptr)
				return index;
		return npos;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::find_first_not_of(const this_type& str, size_type index) const
	{
		for (; index < size(); ++index)
			if (traits_type::find(str.c_str(), str.size(), *(c_str() + index)) == nullptr)
				return index;
		return npos;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::find_last_of(const this_type& str, size_type index) const
	{
		index = simstd::min(index, size());
		while (index > 0)
			if (traits_type::find(str.c_str(), str.size(), *(c_str() + --index)) != nullptr)
				return index;
		return npos;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::find_last_not_of(const this_type& str, size_type index) const
	{
		index = simstd::min(index, size());
		while (index > 0)
			if (traits_type::find(str.c_str(), str.size(), *(c_str() + --index)) == nullptr)
				return index;
		return npos;
	}

	template<typename C, typename T, typename A>
	basic_string<C, T, A>::basic_string(const allocator_type& allocator, size_type capacity, const_pointer str1, size_type len1, const_pointer str2, size_type len2) :
		base_type(allocator, capacity)
	{
		TraceFormatObj("(%Iu, %p, %Iu, %p, %Iu)\n", capacity, str1, len1, str2, len2);
		if (len1)
			_raw_append(str1, len1);
		if (len2)
			_raw_append(str2, len2);
	}

	template<typename C, typename T, typename A>
	template<typename InputIt>
	basic_string<C, T, A>::basic_string(InputIt first, InputIt last, const allocator_type& allocator, simstd::input_iterator_tag):
		base_type(allocator)
	{
		append(first, last);
	}

	template<typename C, typename T, typename A>
	template<typename ForwardIt>
	basic_string<C, T, A>::basic_string(ForwardIt first, ForwardIt last, const allocator_type& allocator, simstd::forward_iterator_tag):
		base_type(allocator, simstd::distance(first, last))
	{
		_raw_append(first, last);
	}

	template<typename C, typename T, typename A>
	void basic_string<C, T, A>::_raw_append(value_type ch, size_type count)
	{
		CRT_ASSERT((size() + count) <= capacity());
		traits_type::assign(impl->get_end(), count, ch);
		impl->set_size(size() + count);
	}

	template<typename C, typename T, typename A>
	void basic_string<C, T, A>::_raw_append(const_pointer str, size_type len)
	{
		CRT_ASSERT((size() + len) <= capacity());
		traits_type::move(impl->get_end(), str, len);
		impl->set_size(size() + len);
	}

	template<typename C, typename T, typename A>
	template<typename InputIt>
	void basic_string<C, T, A>::_raw_append(InputIt first, InputIt last)
	{
		auto ptr = impl->get_end();
		for (; first != last; ++first, ++ptr) {
			traits_type::assign(*ptr, *first);
		}
		CRT_ASSERT((size() + simstd::distance(impl->get_end(), ptr)) <= capacity());
		impl->set_size(size() + simstd::distance(impl->get_end(), ptr));
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::get_necessary_capacity(ptrdiff_t addToSize) const
	{
		if (capacity() < (size() + addToSize))
			return size() + simstd::max((ptrdiff_t)size(), addToSize);
		return capacity();
	}

	template<typename C, typename T, typename A>
	bool basic_string<C, T, A>::must_be_splitted(size_type minCapacity)
	{
		return capacity() < minCapacity || impl->is_shared();
	}

	template<typename C, typename T, typename A>
	void basic_string<C, T, A>::split_and_clear(size_type minCapacity)
	{
		if (must_be_splitted(minCapacity))
			this_type(get_allocator(), simstd::max(minCapacity, capacity()), nullptr, 0).swap(*this);
		else
			impl->set_size(0);
	}

	template<typename C, typename T, typename A>
	void basic_string<C, T, A>::split_and_copy(size_type minCapacity)
	{
		if (must_be_splitted(minCapacity))
			this_type(get_allocator(), simstd::max(minCapacity, capacity()), c_str(), size()).swap(*this);
	}

	template<typename C, typename T, typename A>
	int basic_string<C, T, A>::_compare(const_pointer str1, size_type len1, const_pointer str2, size_type len2)
	{
		auto result = traits_type::compare(str1, str2, simstd::min(len1, len2));
		if (result != 0 || len1 == len2)
			return result;
		return (len1 < len2) ? -1 : 1;
	}

//	template<typename C, typename T, typename A>
//	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::find(const_pointer str, size_type index, size_type length) const
//	{
//		const size_type size = this->size();
//		const_pointer data = this->c_str();
//
//		if (length == 0)
//			return index <= size ? index : npos;
//
//		if (length <= size)
//		{
//			for (; index <= size - length; ++index)
//				if (traits_type::eq(data[index], str[0]) && traits_type::compare(data + index + 1, str + 1, length - 1) == 0)
//					return index;
//		}
//		return npos;
//	}
//
	template<typename C, typename T, typename A>
	bool basic_string<C, T, A>::is_same_str(const_pointer str) const
	{
		return c_str() <= str && str <= (c_str() + size());
	}

	///=============================================================================================
	template<typename C, typename T, typename A>
	basic_string<C, T, A> operator +(const basic_string<C, T, A>& a, const basic_string<C, T, A>& b)
	{
		return basic_string<C, T, A>(a.get_allocator(), a.size() + b.size(), a.c_str(), a.size(), b.c_str(), b.size());
	}

	template<typename C, typename T, typename A>
	basic_string<C, T, A> operator +(const C* a, const basic_string<C, T, A>& b)
	{
		using traits_type = typename basic_string<C, T, A>::traits_type;
		auto length = traits_type::length(a);
		return basic_string<C, T, A>(b.get_allocator(), length + b.size(), a, length, b.c_str(), b.size());
	}

	template<typename C, typename T, typename A>
	basic_string<C, T, A> operator +(C a, const basic_string<C, T, A>& b)
	{
		return basic_string<C, T, A>(b.get_allocator(), 1 + b.size(), &a, 1, b.c_str(), b.size());
	}

	template<typename C, typename T, typename A>
	basic_string<C, T, A> operator +(const basic_string<C, T, A>& a, const C* b)
	{
		using traits_type = typename basic_string<C, T, A>::traits_type;
		auto length = traits_type::length(b);
		return basic_string<C, T, A>(a.get_allocator(), a.size() + length, a.c_str(), a.size(), b, length);
	}

	template<typename C, typename T, typename A>
	basic_string<C, T, A> operator +(const basic_string<C, T, A>& a, C b)
	{
		return basic_string<C, T, A>(a.get_allocator(), a.size() + 1, a.c_str(), a.size(), &b, 1);
	}

	template<typename C, typename T, typename A>
	bool operator ==(const basic_string<C, T, A>& a, const basic_string<C, T, A>& b)
	{
		return basic_string<C, T, A>::_compare(a.c_str(), a.size(), b.c_str(), b.size()) == 0;
	}

	template<typename C, typename T, typename A>
	bool operator ==(const C* a, const basic_string<C, T, A>& b)
	{
		using traits_type = typename basic_string<C, T, A>::traits_type;
		return basic_string<C, T, A>::_compare(a, traits_type::length(a), b.c_str(), b.size()) == 0;
	}

	template<typename C, typename T, typename A>
	bool operator ==(const basic_string<C, T, A>& a, const C* b)
	{
		using traits_type = typename basic_string<C, T, A>::traits_type;
		return basic_string<C, T, A>::_compare(a.c_str(), a.size(), b, traits_type::length(b)) == 0;
	}

	template<typename C, typename T, typename A>
	bool operator !=(const basic_string<C, T, A>& a, const basic_string<C, T, A>& b)
	{
		return rel_ops::operator !=(a, b);
	}

	template<typename C, typename T, typename A>
	bool operator !=(const C* a, const basic_string<C, T, A>& b)
	{
		return rel_ops::operator !=(a, b);
	}

	template<typename C, typename T, typename A>
	bool operator !=(const basic_string<C, T, A>& a, const C* b)
	{
		return rel_ops::operator !=(a, b);
	}

	template<typename C, typename T, typename A>
	bool operator <(const basic_string<C, T, A>& a, const basic_string<C, T, A>& b)
	{
		return basic_string<C, T, A>::_compare(a.c_str(), a.size(), b.c_str(), b.size()) < 0;
	}

	template<typename C, typename T, typename A>
	bool operator <(const C* a, const basic_string<C, T, A>& b)
	{
		using traits_type = typename basic_string<C, T, A>::traits_type;
		return basic_string<C, T, A>::_compare(a, traits_type::length(a), b.c_str(), b.size()) < 0;
	}

	template<typename C, typename T, typename A>
	bool operator <(const basic_string<C, T, A>& a, const C* b)
	{
		using traits_type = typename basic_string<C, T, A>::traits_type;
		return basic_string<C, T, A>::_compare(a.c_str(), a.size(), b, traits_type::length(b)) < 0;
	}

	template<typename C, typename T, typename A>
	bool operator <=(const basic_string<C, T, A>& a, const basic_string<C, T, A>& b)
	{
		return rel_ops::operator <=(a, b);
	}

	template<typename C, typename T, typename A>
	bool operator <=(const C* a, const basic_string<C, T, A>& b)
	{
		return rel_ops::operator <=(a, b);
	}

	template<typename C, typename T, typename A>
	bool operator <=(const basic_string<C, T, A>& a, const C* b)
	{
		return rel_ops::operator <=(a, b);
	}

	template<typename C, typename T, typename A>
	bool operator >(const basic_string<C, T, A>& a, const basic_string<C, T, A>& b)
	{
		return rel_ops::operator >(a, b);
	}

	template<typename C, typename T, typename A>
	bool operator >(const C* a, const basic_string<C, T, A>& b)
	{
		return rel_ops::operator >(a, b);
	}

	template<typename C, typename T, typename A>
	bool operator >(const basic_string<C, T, A>& a, const C* b)
	{
		return rel_ops::operator >(a, b);
	}

	template<typename C, typename T, typename A>
	bool operator >=(const basic_string<C, T, A>& a, const basic_string<C, T, A>& b)
	{
		return rel_ops::operator >=(a, b);
	}

	template<typename C, typename T, typename A>
	bool operator >=(const C* a, const basic_string<C, T, A>& b)
	{
		return rel_ops::operator >=(a, b);
	}

	template<typename C, typename T, typename A>
	bool operator >=(const basic_string<C, T, A>& a, const C* b)
	{
		return rel_ops::operator >=(a, b);
	}

	template<typename C, typename T, typename A>
	void swap(basic_string<C, T, A>& a, basic_string<C, T, A>& b)
	{
		a.swap(b);
	}

}

#endif
