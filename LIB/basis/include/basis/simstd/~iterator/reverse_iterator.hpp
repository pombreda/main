#ifndef BASIS_SIMSTD_ITERATOR_REVERSE_ITERATOR_HPP_
#define BASIS_SIMSTD_ITERATOR_REVERSE_ITERATOR_HPP_

namespace simstd
{

	template<typename Pointer>
	class reverse_iterator
	{
		using iterator_type = Pointer;
		using this_type = reverse_iterator<iterator_type>;
		using traits_type = iterator_traits<iterator_type>;

	public:
		using iterator_category = typename traits_type::iterator_category;
		using value_type        = typename traits_type::value_type;
		using difference_type   = typename traits_type::difference_type;
		using reference         = typename traits_type::reference;
		using pointer           = typename traits_type::pointer;

		reverse_iterator() : current() {}
		explicit reverse_iterator(iterator_type ptr) : current(ptr) {}
		reverse_iterator(const this_type& other) : current(other.current) {}

		template <typename OType>
		reverse_iterator(const reverse_iterator<OType>& ri) : current(ri.base()) {}

		template <typename OType>
		reverse_iterator<iterator_type>& operator =(const reverse_iterator<OType>& ri) {current = ri.base(); return *this;}

		iterator_type base() const                      {return current;}

		// Forward iterator requirements
		reference  operator * () const                  {iterator_type tmp = current; return *--tmp;}
		pointer    operator ->() const                  {return &(operator*());}
		this_type& operator ++()                        {--current; return *this;}
		this_type  operator ++(int)                     {this_type tmp(*this); --current; return tmp;}

		// Bidirectional iterator requirements
		this_type& operator --()                        {++current; return *this;}
		this_type  operator --(int)                     {this_type tmp(*this); ++current; return tmp;}

		// Random access iterator requirements
		reference  operator [](difference_type n) const {return *(*this + n);}
		this_type  operator + (difference_type n) const {return this_type(current - n);}
		this_type& operator +=(difference_type n)       {current -= n; return *this;}
		this_type  operator - (difference_type n) const {return this_type(current + n);}
		this_type& operator -=(difference_type n)       {current += n; return *this;}

	protected:
		iterator_type current;
	};

	template<typename Type1, typename Type2>
	bool operator ==(const reverse_iterator<Type1>& lhs, const reverse_iterator<Type2>& rhs)
	{
		return lhs.base() == rhs.base();
	}

	template<typename Type1, typename Type2>
	bool operator <(const reverse_iterator<Type1>& lhs, const reverse_iterator<Type2>& rhs)
	{
		return lhs.base() < rhs.base();
	}

	template<typename Type1, typename Type2>
	bool operator !=(const reverse_iterator<Type1>& lhs, const reverse_iterator<Type2>& rhs)
	{
		return simstd::rel_ops::operator !=(lhs, rhs);
	}

	template<typename Type1, typename Type2>
	bool operator <=(const reverse_iterator<Type1>& lhs, const reverse_iterator<Type2>& rhs)
	{
		return simstd::rel_ops::operator <=(lhs, rhs);
	}

	template<typename Type1, typename Type2>
	bool operator >(const reverse_iterator<Type1>& lhs, const reverse_iterator<Type2>& rhs)
	{
		return simstd::rel_ops::operator >(lhs, rhs);
	}

	template<typename Type1, typename Type2>
	bool operator >=(const reverse_iterator<Type1>& lhs, const reverse_iterator<Type2>& rhs)
	{
		return simstd::rel_ops::operator >=(lhs, rhs);
	}

	template<typename Type>
	reverse_iterator<Type> operator +(typename reverse_iterator<Type>::difference_type n, const reverse_iterator<Type> & it)
	{
		return it + n;
	}

	template<typename Type>
	typename
	reverse_iterator<Type>::difference_type operator -(const reverse_iterator<Type>& lhs, const reverse_iterator<Type>& rhs)
	{
		return rhs.base() - lhs.base();
	}

}

#endif
