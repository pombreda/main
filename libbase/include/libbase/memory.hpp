﻿#ifndef _LIBBASE_MEMORY_HPP_
#define _LIBBASE_MEMORY_HPP_

#include <libbase/std.hpp>
#include <libbase/uncopyable.hpp>

namespace Base {

	template<typename Type>
	inline Type & reverse_bytes(Type & inout)
	{
		std::reverse((char*)&inout, ((char*)&inout) + sizeof(inout));
		return inout;
	}

	inline WORD & swap_bytes(WORD & inout)
	{
		inout = inout >> 8 | inout << 8;
		return inout;
	}

	inline DWORD & swap_words(DWORD &inout)
	{
		inout = inout >> 16 | inout << 16;
		return inout;
	}

	inline intmax_t mega_to_bytes(size_t in)
	{
		return (in != 0) ? (intmax_t)in << 20 : -1ll;
	}

	inline size_t bytes_to_mega(intmax_t in)
	{
		return (in > 0) ? in >> 20 : 0;
	}

	///▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒ constraints
	template<typename Type>
	struct must_be_pointer {
		static bool constraints(const Type & type_is_not_pointer)
		{
			return sizeof(0[type_is_not_pointer]);
		}
	};

	template<>
	struct must_be_pointer<PVOID> {
		static bool constraints(const PVOID &)
		{
			return true;
		}
	};

	///==================================================================================== auto_buf
	template<typename Type>
	struct auto_buf: private Uncopyable {
		typedef auto_buf this_type;
		typedef Type value_type;
		typedef size_t size_type;

		~auto_buf()
		{
			must_be_pointer<Type>::constraints(m_ptr);
			Memory::free(m_ptr);
		}

		auto_buf() :
			m_ptr(nullptr)
		{
		}

		explicit auto_buf(size_type size) :
			m_ptr(Memory::malloc<value_type>(size, 0))
		{
		}

		auto_buf(this_type && rhs):
			m_ptr(nullptr)
		{
			swap(rhs);
		}

		this_type & operator =(this_type && rhs)
		{
			if (this != &rhs)
				this_type(rhs).swap(*this);
			return *this;
		}

		void reserve(size_type nsize)
		{
			if (size() < nsize)
			{
				Memory::realloc(m_ptr, nsize);
			}
		}

		size_type size() const
		{
			return Memory::size(m_ptr);
		}

		value_type operator &() const
		{
			return m_ptr;
		}

		value_type operator ->() const
		{
			return m_ptr;
		}

		operator value_type() const
		{
			return m_ptr;
		}

		value_type data() const
		{
			return m_ptr;
		}

		bool operator !() const
		{
			return m_ptr;
		}

		void attach(value_type & ptr)
		{
			Memory::free(m_ptr);
			m_ptr = ptr;
		}

		void detach(value_type & ptr)
		{
			ptr = m_ptr;
			m_ptr = nullptr;
		}

		void swap(value_type & ptr) noexcept
		{
			using std::swap;
			swap(m_ptr, ptr);
		}

		void swap(this_type & rhs) noexcept
		{
			using std::swap;
			swap(m_ptr, rhs.m_ptr);
		}

	private:
		value_type m_ptr;
	};

	template<typename Type>
	void swap(auto_buf<Type> & b1, auto_buf<Type> & b2)
	{
		b1.swap(b2);
	}

	///==================================================================================== auto_buf
	template<typename Type>
	class auto_array: private Uncopyable {
		typedef auto_array this_type;
		typedef Type value_type;
		typedef Type * pointer_type;
		typedef size_t size_type;

	public:
		~auto_array()
		{
			Memory::free(m_ptr);
		}

		explicit auto_array(size_type count, const Type * data = nullptr) :
			m_ptr(Memory::calloc<pointer_type>(count)),
			m_count(count)
		{
			if (data)
				std::copy(data, data + m_count, m_ptr);
		}

		auto_array(this_type && rhs):
			m_ptr(nullptr),
			m_count(0)
		{
			swap(rhs);
		}

		this_type & operator =(this_type && rhs)
		{
			if (this != &rhs)
				this_type(std::move(rhs)).swap(*this);
			return *this;
		}

		void reserve(size_type new_count)
		{
			if (size() < new_count)
			{
				Memory::realloc(m_ptr, new_count * sizeof(Type), HEAP_ZERO_MEMORY);
				m_count = new_count;
			}
		}

		size_type size() const
		{
			return m_count;
		}

		size_type size_in_bytes() const
		{
			return m_count * sizeof(Type);
		}

		operator pointer_type() const
		{
			return m_ptr;
		}

		pointer_type data() const
		{
			return m_ptr;
		}

		value_type & operator [](int ind)
		{
			return m_ptr[ind];
		}

		const value_type & operator [](int ind) const
		{
			return m_ptr[ind];
		}

		bool operator ==(const this_type & rhs) const
		{
			return (m_count == rhs.m_count) ?
				std::equal(m_ptr, m_ptr + m_count, rhs.m_ptr)
			:
				false;
		}

		void detach(pointer_type & ptr, size_t & size)
		{
			ptr = m_ptr;
			size = m_count;
			m_ptr = nullptr;
			m_count = 0;
		}

		void swap(this_type & rhs)
		{
			using std::swap;
			swap(m_ptr, rhs.m_ptr);
			swap(m_count, rhs.m_count);
		}

	private:
		pointer_type m_ptr;
		size_type m_count;
	};

	template<typename Type>
	void swap(auto_array<Type> & b1, auto_array<Type> & b2)
	{
		b1.swap(b2);
	}

	///================================================================================== auto_close
	template<typename Type>
	class auto_close: private Uncopyable {
		typedef Type value_type;

	public:
		~auto_close()
		{
			delete m_impl;
		}

		explicit auto_close(value_type ptr) :
				m_impl(new auto_close_impl(ptr))
		{
		}

		template<typename Deleter>
		explicit auto_close(value_type ptr, Deleter del) :
			m_impl(new auto_close_deleter_impl<Deleter>(ptr, del))
		{
		}

		operator value_type()
		{
			return m_impl->m_ptr;
		}

		value_type * operator &()
		{
			return &(m_impl->m_ptr);
		}

		void swap(auto_close & rhs)
		{
			using std::swap;
			swap(m_impl, rhs.m_impl);
		}

	private:
		struct auto_close_impl {
			auto_close_impl(value_type ptr) :
				m_ptr(ptr)
			{
			}

			virtual ~auto_close_impl()
			{
//				delete m_ptr;
			}
			value_type m_ptr;
		};

		template<typename Deleter>
		struct auto_close_deleter_impl: public auto_close_impl {
			auto_close_deleter_impl(value_type ptr, Deleter d) :
				auto_close_impl(ptr), m_deleter(d)
			{
			}
			virtual ~auto_close_deleter_impl()
			{
				m_deleter(this->m_ptr);
//				this->m_ptr = nullptr;
			}
			Deleter m_deleter;
		};

		auto_close_impl * m_impl;
	};

	template<typename Type>
	inline void swap(auto_close<Type> & b1, auto_close<Type> & b2)
	{
		b1.swap(b2);
	}

	template<>
	struct auto_close<HANDLE> {
		typedef auto_close this_type;
		typedef HANDLE value_type;

		~auto_close()
		{
			close();
		}

		explicit auto_close(value_type ptr = nullptr) :
			m_ptr(ptr)
		{
		}

		auto_close(const this_type & rhs);

		auto_close(this_type && rhs);

		this_type & operator = (const this_type & rhs);

		this_type & operator = (this_type && rhs);

		value_type * operator & ();

		operator value_type() const
		{
			return m_ptr;
		}

		value_type data() const
		{
			return m_ptr;
		}

		bool is_valid() const
		{
			return m_ptr && m_ptr != INVALID_HANDLE_VALUE;
		}

		void close();

		void swap(auto_close & rhs)
		{
			using std::swap;
			swap(m_ptr, rhs.m_ptr);
		}

	private:
		value_type m_ptr;
	};

	inline void swap(auto_close<HANDLE> & b1, auto_close<HANDLE> & b2)
	{
		b1.swap(b2);
	}

	///================================================================================ auto_destroy
	struct Destroyable {
		virtual ~Destroyable();

		virtual void destroy() const = 0;
	};

	template<typename Type>
	struct auto_destroy {
		~auto_destroy()
		{
			m_ptr->destroy();
		}

		auto_destroy(Type ptr) :
			m_ptr(ptr)
		{
		}

		Type operator ->()
		{
			return m_ptr;
		}

		const Type operator ->() const
		{
			return m_ptr;
		}

	private:
		Type m_ptr;
	};
}

#endif
