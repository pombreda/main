#ifndef LIBBASE_CSTR_MSTRING_HPP_
#define LIBBASE_CSTR_MSTRING_HPP_

namespace cstr {

	class mstring {
		typedef mstring this_type;

	public:
		typedef wchar_t           value_type;
		typedef size_t            size_type;
		typedef value_type&       reference;
		typedef const value_type& const_reference;
		typedef value_type*       pointer;
		typedef const value_type* const_pointer;

		~mstring();

		mstring(const_pointer in = EMPTY_STR);

		mstring(const this_type& other);

		mstring& operator =(const this_type& other);

		mstring(this_type&& rhs);

		mstring& operator =(this_type&& rhs);

		void push_back(const_pointer str);

		size_type size() const;

		size_type capacity() const;

		const_pointer c_str() const;

		const_pointer operator [](size_type index) const;

		void swap(this_type& other);

		static ssize_t unit_test();

	private:
		void add_data(const_pointer mstr);

		pointer   m_data;
		size_type m_capa;
		size_type m_size;
	};

}

#endif
