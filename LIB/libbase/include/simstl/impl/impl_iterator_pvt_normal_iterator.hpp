#ifndef SARALIB_STL_ITERATOR_NORMAL_ITERATOR_HPP_
#define SARALIB_STL_ITERATOR_NORMAL_ITERATOR_HPP_

#include "impl_types.hpp"
#include "impl_iterator_base.hpp"
#include "impl_utility.hpp"

namespace sarastd
{
	namespace pvt {
		template<typename Iterator>
		class _normal_iterator
		{
			typedef Iterator                                iterator_type;
			typedef _normal_iterator<Iterator>              this_type;
			typedef iterator_traits<Iterator>               traits_type;

		public:
			typedef typename traits_type::iterator_category iterator_category;
			typedef typename traits_type::value_type        value_type;
			typedef typename traits_type::difference_type   difference_type;
			typedef typename traits_type::pointer           pointer;
			typedef typename traits_type::reference         reference;

			_normal_iterator() : current() {}
			explicit _normal_iterator(Iterator it) : current(it) {}
			_normal_iterator(const this_type& ri) : current(ri.current) {}

			template <typename U>
			_normal_iterator(const _normal_iterator<U>& ri) : current(ri.base()) {}

			template <typename U>
			_normal_iterator<Iterator>& operator =(const _normal_iterator<U>& ri) {current = ri.base(); return *this;}

			iterator_type base() const     {return current;}

			// Forward iterator requirements
			reference  operator * () const {return *current;}
			pointer    operator ->() const {return current;}
			this_type& operator ++()       {++current; return *this;}
			this_type  operator ++(int)    {return this_type(current++);}

			// Bidirectional iterator requirements
			this_type& operator --()       {--current; return *this;}
			this_type  operator --(int)    {return this_type(current--);}

			// Random access iterator requirements
			reference  operator [](const difference_type& n) const {return current[n];}
			this_type& operator +=(const difference_type& n)       {current += n; return *this;}
			this_type  operator +(const difference_type& n) const  {return this_type(current + n);}
			this_type& operator -=(const difference_type& n)       {current -= n; return *this;}
			this_type  operator -(const difference_type& n) const  {return this_type(current - n);}

		protected:
			iterator_type current;
		};

		template<typename Iterator1, typename Iterator2>
		bool operator ==(const _normal_iterator<Iterator1>& lhs, const _normal_iterator<Iterator2>& rhs)
		{
			return lhs.base() == rhs.base();
		}

		template<typename Iterator1, typename Iterator2>
		bool operator !=(const _normal_iterator<Iterator1>& lhs, const _normal_iterator<Iterator2>& rhs)
		{
			return sarastd::pvt::ops::operator !=(lhs, rhs);
		}

		template<typename Iterator1, typename Iterator2>
		bool operator <(const _normal_iterator<Iterator1>& lhs, const _normal_iterator<Iterator2>& rhs)
		{
			return lhs.base() < rhs.base();
		}

		template<typename Iterator1, typename Iterator2>
		bool operator <=(const _normal_iterator<Iterator1>& lhs, const _normal_iterator<Iterator2>& rhs)
		{
			return sarastd::pvt::ops::operator <=(lhs, rhs);
		}

		template<typename Iterator1, typename Iterator2>
		bool operator >(const _normal_iterator<Iterator1>& lhs, const _normal_iterator<Iterator2>& rhs)
		{
			return sarastd::pvt::ops::operator >(lhs, rhs);
		}

		template<typename Iterator1, typename Iterator2>
		bool operator >=(const _normal_iterator<Iterator1>& lhs, const _normal_iterator<Iterator2>& rhs)
		{
			return sarastd::pvt::ops::operator >=(lhs, rhs);
		}

		template<typename Iterator>
		_normal_iterator<Iterator> operator +(typename _normal_iterator<Iterator>::difference_type n, const _normal_iterator<Iterator> & it)
		{
			return it + n;
		}

		template<typename Iterator>
		typename
		_normal_iterator<Iterator>::difference_type operator -(const _normal_iterator<Iterator>& lhs, const _normal_iterator<Iterator>& rhs)
		{
			return lhs.base() - rhs.base();
		}
	}
}

#endif