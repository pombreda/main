﻿#ifndef BASIS_MEMORY_UNIQUE_PTR_HPP_
#define BASIS_MEMORY_UNIQUE_PTR_HPP_

namespace simstd
{
	template<typename Type, typename Deleter>
	class unique_ptr: private pvt::ebo_helper<0, Deleter> {
		using helper_type = pvt::ebo_helper<0, Deleter>;

	public:
		using pointer = typename pvt::RecognizePointer<Type, Deleter>::type;
		using element_type = Type;
		using deleter_type = Deleter;

	private:
		using condition_type_r = typename conditional<is_reference<deleter_type>::value, deleter_type, const deleter_type&>::type;
		using condition_type_ur = remove_reference_t<deleter_type>&&;

	public:
		~unique_ptr() noexcept;

		unique_ptr() noexcept;
		unique_ptr(nullptr_t) noexcept;
		explicit  unique_ptr(pointer ptr) noexcept;

		template<typename OType, typename ODeleter>
		unique_ptr(unique_ptr<OType, ODeleter> && other) noexcept;
		unique_ptr(unique_ptr&& other) noexcept;
		unique_ptr(pointer ptr, condition_type_r deleter) noexcept;
		unique_ptr(pointer ptr, condition_type_ur deleter) noexcept;

		template<typename OType, typename ODeleter>
		unique_ptr& operator =(unique_ptr<OType, ODeleter>&& other) noexcept;
		unique_ptr& operator =(unique_ptr&& other) noexcept;
		unique_ptr& operator =(nullptr_t) noexcept;

		typename defstd::add_lvalue_reference<element_type>::type operator *() const noexcept
		{
			CRT_ASSERT(m_ptr);
			return *get();
		}

		pointer operator ->() const noexcept;

		pointer get() const noexcept;

		deleter_type& get_deleter() noexcept;
		const deleter_type& get_deleter() const noexcept;

		explicit operator bool() const noexcept;

		pointer release() noexcept;

		void reset(pointer ptr = pointer()) noexcept;

		void swap(unique_ptr& other) noexcept;

	private:
		unique_ptr(const unique_ptr&) = delete;
		unique_ptr& operator =(const unique_ptr&) = delete;

		pointer m_ptr;
	};
}

namespace simstd
{
	template<typename Type, typename Deleter>
	unique_ptr<Type, Deleter>::~unique_ptr() noexcept
	{
		TraceObj();
		if (get() != pointer())
			get_deleter()(get());
		m_ptr = pointer();
	}

	template<typename Type, typename Deleter>
	unique_ptr<Type, Deleter>::unique_ptr() noexcept
		: helper_type(deleter_type())
		, m_ptr()
	{
		TraceObj();
		static_assert(!is_pointer<deleter_type>::value, "constructed with null function pointer deleter");
	}

	template<typename Type, typename Deleter>
	unique_ptr<Type, Deleter>::unique_ptr(nullptr_t) noexcept
		: helper_type(deleter_type())
		, m_ptr()
	{
		TraceObj();
	}

	template<typename Type, typename Deleter>
	unique_ptr<Type, Deleter>::unique_ptr(pointer ptr) noexcept
		: helper_type(deleter_type())
		, m_ptr(ptr)
	{
		TraceObj();
	}

	template<typename Type, typename Deleter>
//	template<typename OType, typename ODeleter, typename = defstd::_Require<defstd::is_convertible<typename unique_ptr<OType, ODeleter>::pointer, pointer>,defstd::__not_<defstd::is_array<OType>>, typename defstd::conditional<defstd::is_reference<Deleter>::value, defstd::is_same<ODeleter, Deleter>, defstd::is_convertible<ODeleter, Deleter>>::type>>
	template<typename OType, typename ODeleter>
	unique_ptr<Type, Deleter>::unique_ptr(unique_ptr<OType, ODeleter>&& other) noexcept
		: helper_type(forward<ODeleter>(other.get_deleter()))
		, m_ptr(other.release())
	{
		TraceObj();
	}

	template<typename Type, typename Deleter>
	unique_ptr<Type, Deleter>::unique_ptr(pointer ptr, condition_type_r dltr) noexcept
		: helper_type(dltr)
		, m_ptr(ptr)
	{
		TraceObj();
	}

	template<typename Type, typename Deleter>
	unique_ptr<Type, Deleter>::unique_ptr(pointer ptr, condition_type_ur dltr) noexcept
		: helper_type(move(dltr))
		, m_ptr(ptr)
	{
		TraceObj();
		static_assert(!is_reference<deleter_type>::value, "rvalue deleter bound to reference");
	}

	template<typename Type, typename Deleter>
	unique_ptr<Type, Deleter>::unique_ptr(unique_ptr&& other) noexcept
		: helper_type(forward<deleter_type>(other.get_deleter()))
		, m_ptr(other.release())
	{
		TraceObj();
	}

	template<typename Type, typename Deleter>
	template<typename OType, typename ODeleter>
	unique_ptr<Type, Deleter>& unique_ptr<Type, Deleter>::operator =(unique_ptr<OType, ODeleter>&& other) noexcept
	{
		TraceObj();
		get_deleter() = forward<ODeleter>(other.get_deleter());
		reset(other.release());
		return *this;
	}
//	template<typename OType, typename ODeleter>
//	typename enable_if< __and_<is_convertible<typename unique_ptr<OType, ODeleter>::pointer, pointer>, __not_<is_array<OType>>>::value, unique_ptr&>::type
//	operator = (unique_ptr<OType, ODeleter>&& other) noexcept
//	{
//		reset(other.release());
//		get_deleter() = forward<ODeleter>(other.get_deleter());
//		return *this;
//	}

	template<typename Type, typename Deleter>
	unique_ptr<Type, Deleter>& unique_ptr<Type, Deleter>::operator =(unique_ptr&& other) noexcept
	{
		TraceObj();
		get_deleter() = forward<deleter_type>(other.get_deleter());
		reset(other.release());
		return *this;
	}

	template<typename Type, typename Deleter>
	unique_ptr<Type, Deleter>& unique_ptr<Type, Deleter>::operator =(nullptr_t) noexcept
	{
		reset();
		return *this;
	}

	template<typename Type, typename Deleter>
	typename unique_ptr<Type, Deleter>::pointer unique_ptr<Type, Deleter>::operator ->() const noexcept
	{
		CRT_ASSERT(m_ptr);
		return get();
	}

	template<typename Type, typename Deleter>
	typename unique_ptr<Type, Deleter>::pointer unique_ptr<Type, Deleter>::get() const noexcept
	{
		return m_ptr;
	}

	template<typename Type, typename Deleter>
	typename unique_ptr<Type, Deleter>::deleter_type& unique_ptr<Type, Deleter>::get_deleter() noexcept
	{
		return helper_type::get(*this);
	}

	template<typename Type, typename Deleter>
	const typename unique_ptr<Type, Deleter>::deleter_type& unique_ptr<Type, Deleter>::get_deleter() const noexcept
	{
		return helper_type::get(*this);
	}

	template<typename Type, typename Deleter>
	unique_ptr<Type, Deleter>::operator bool() const noexcept
	{
		return get() != pointer();
	}

	template<typename Type, typename Deleter>
	typename unique_ptr<Type, Deleter>::pointer unique_ptr<Type, Deleter>::release() noexcept
	{
		TraceObj();
		using simstd::swap;
		pointer ptr = pointer();
		swap(m_ptr, ptr);
		return ptr;
	}

	template<typename Type, typename Deleter>
	void unique_ptr<Type, Deleter>::reset(pointer ptr) noexcept
	{
		TraceObj();
		using simstd::swap;
		swap(m_ptr, ptr);
		if (ptr != pointer())
			get_deleter()(ptr);
	}

	template<typename Type, typename Deleter>
	void unique_ptr<Type, Deleter>::swap(unique_ptr& other) noexcept
	{
		TraceObj();
		using simstd::swap;
		swap(static_cast<helper_type&>(*this), static_cast<helper_type&>(other));
		swap(m_ptr, other.m_ptr);
	}
}

namespace simstd
{
	template<typename Type, typename Deleter>
	void swap(unique_ptr<Type, Deleter>& a, unique_ptr<Type, Deleter>& b) noexcept
	{
		a.swap(b);
	}

	template<typename Type, typename Deleter, typename OType, typename ODeleter>
	bool operator ==(const unique_ptr<Type, Deleter>& a, const unique_ptr<OType, ODeleter>& b) noexcept
	{
		return a.get() == b.get();
	}

	template<typename Type, typename Deleter>
	bool operator ==(const unique_ptr<Type, Deleter>& a, nullptr_t) noexcept
	{
		return !a;
	}

	template<typename Type, typename Deleter>
	bool operator ==(nullptr_t, const unique_ptr<Type, Deleter>& b) noexcept
	{
		return !b;
	}

	template<typename Type, typename Deleter, typename OType, typename ODeleter>
	bool operator <(const unique_ptr<Type, Deleter>& a, const unique_ptr<OType, ODeleter>& b) noexcept
	{
		typedef typename defstd::common_type<typename unique_ptr<Type, Deleter>::pointer, typename unique_ptr<OType, ODeleter>::pointer>::type CommonType;
		return less<CommonType>()(a.get(), b.get());
	}

	template<typename Type, typename Deleter>
	bool operator <(const unique_ptr<Type, Deleter>& a, nullptr_t) noexcept
	{
		return less<typename unique_ptr<Type, Deleter>::pointer>()(a.get(), nullptr);
	}

	template<typename Type, typename Deleter>
	bool operator <(nullptr_t, const unique_ptr<Type, Deleter>& a) noexcept
	{
		return less<typename unique_ptr<Type, Deleter>::pointer>()(nullptr, a.get());
	}

	template<typename Type, typename Deleter, typename OType, typename ODeleter>
	bool operator !=(const unique_ptr<Type, Deleter>& a, const unique_ptr<OType, ODeleter>& b) noexcept
	{
		return rel_ops::operator !=(a, b);
	}

	template<typename Type, typename Deleter>
	bool operator !=(const unique_ptr<Type, Deleter>& a, nullptr_t) noexcept
	{
		return rel_ops::operator !=(a, nullptr);
	}

	template<typename Type, typename Deleter>
	bool operator !=(nullptr_t, const unique_ptr<Type, Deleter>& b) noexcept
	{
		return rel_ops::operator !=(nullptr, b);
	}

	template<typename Type, typename Deleter, typename OType, typename ODeleter>
	bool operator <=(const unique_ptr<Type, Deleter>& a, const unique_ptr<OType, ODeleter>& b) noexcept
	{
		return rel_ops::operator <=(a, b);
	}

	template<typename Type, typename Deleter>
	bool operator <=(const unique_ptr<Type, Deleter>& a, nullptr_t) noexcept
	{
		return rel_ops::operator <=(a, nullptr);
	}

	template<typename Type, typename Deleter>
	bool operator <=(nullptr_t, const unique_ptr<Type, Deleter>& b) noexcept
	{
		return rel_ops::operator <=(nullptr, b);
	}

	template<typename Type, typename Deleter, typename OType, typename ODeleter>
	bool operator >(const unique_ptr<Type, Deleter>& a, const unique_ptr<OType, ODeleter>& b) noexcept
	{
		return rel_ops::operator >(a, b);
	}

	template<typename Type, typename Deleter>
	bool operator >(const unique_ptr<Type, Deleter>& a, nullptr_t) noexcept
	{
		return rel_ops::operator >(a, nullptr);
	}

	template<typename Type, typename Deleter>
	bool operator >(nullptr_t, const unique_ptr<Type, Deleter>& b) noexcept
	{
		return rel_ops::operator >(nullptr, b);
	}

	template<typename Type, typename Deleter, typename OType, typename ODeleter>
	bool operator >=(const unique_ptr<Type, Deleter>& a, const unique_ptr<OType, ODeleter>& b) noexcept
	{
		return rel_ops::operator >=(a, b);
	}

	template<typename Type, typename Deleter>
	bool operator >=(const unique_ptr<Type, Deleter>& a, nullptr_t) noexcept
	{
		return rel_ops::operator >=(a, nullptr);
	}

	template<typename Type, typename Deleter>
	bool operator >=(nullptr_t, const unique_ptr<Type, Deleter>& b) noexcept
	{
		return rel_ops::operator >=(nullptr, b);
	}

	template<typename Type, typename... Args>
	unique_ptr<Type> make_unique(Args&&... args)
	{
		TraceFunc();
		return unique_ptr<Type>(new Type(forward<Args>(args)...));
	}
}

#endif
