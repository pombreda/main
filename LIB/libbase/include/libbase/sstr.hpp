#ifndef LIBBASE_SSTR_HPP_
#define LIBBASE_SSTR_HPP_

#include <libbase/std.hpp>

namespace Base {

	class sstr {
		typedef sstr this_type;
		typedef wchar_t value_type;

	public:
		typedef size_t size_type;
		typedef value_type & reference;
		typedef const value_type & const_reference;
		typedef value_type * pointer;
		typedef const value_type * const_pointer;

		~sstr();

		sstr();

		sstr(const_pointer ptr);

		sstr(const this_type & other);

		sstr(this_type && other);

		sstr & operator =(const this_type & other);

		sstr & operator =(this_type && other);

		reference operator [](size_type index);

		const_reference operator [](size_type index) const;

		const_pointer c_str() const;

		bool empty() const;

		size_type size() const;

		void swap(this_type & other);

		static sstr format(const_pointer fmt, va_list args);

		static sstr format(const_pointer fmt, ...);

	private:
		sstr(pointer ptr, bool capture);

		static pointer make_copy(const_pointer ptr, size_type size);

		static pointer make_copy(const_pointer ptr);

		static void free(pointer ptr);

		pointer m_data;
	};

	class mstring {
		typedef mstring this_type;

	public:
		typedef wchar_t value_type;
		typedef size_t size_type;
		typedef value_type & reference;
		typedef const value_type & const_reference;
		typedef value_type * pointer;
		typedef const value_type * const_pointer;

		~mstring();

		mstring(const_pointer in = Base::EMPTY_STR);

		mstring(const this_type & other);

		mstring & operator =(const this_type & other);

		mstring(this_type && rhs);

		mstring & operator =(this_type && rhs);

		void push_back(const_pointer str);

		size_type size() const;

		size_type capacity() const;

		const_pointer c_str() const;

		const_pointer operator [](size_type index) const;

		void swap(this_type & other);

	private:
		void add_data(const_pointer mstr);

		pointer   m_data;
		size_type m_capa;
		size_type m_size;
	};
}

#endif
