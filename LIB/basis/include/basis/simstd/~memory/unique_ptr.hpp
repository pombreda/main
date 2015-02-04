#ifndef BASIS_MEMORY_UNIQUE_PTR_HPP_
#define BASIS_MEMORY_UNIQUE_PTR_HPP_

#include <basis/simstd/~memory/default_deleters.hpp>
#include <basis/simstd/~functional/functional.hpp>

#include <type_traits>

namespace simstd {

	template<typename Type, typename Deleter = default_delete<Type>>
	class unique_ptr {
		class Pointer {
			using DeleterType = typename simstd::remove_reference<Deleter>::type;

			template<typename TypeI>
			static typename TypeI::pointer test_pointer(typename TypeI::pointer*);

			template<typename TypeI>
			static Type* test_pointer(...);

		public:
			typedef decltype(test_pointer<DeleterType>(0)) type;
		};

	public:
		using pointer = typename Pointer::type;
		using element_type = Type;
		using deleter_type = Deleter;

	private:
		using condition_type_r = typename std::conditional<std::is_reference<deleter_type>::value, deleter_type, const deleter_type&>::type;
		using condition_type_ur = typename simstd::remove_reference<deleter_type>::type&&;

	public:
		~unique_ptr() noexcept;

		constexpr unique_ptr() noexcept;
		constexpr unique_ptr(nullptr_t) noexcept;
		explicit  unique_ptr(pointer ptr) noexcept;

		template<typename OType, typename ODeleter>
		unique_ptr(unique_ptr<OType, ODeleter> && other) noexcept;
		unique_ptr(pointer ptr, condition_type_r dltr) noexcept;
		unique_ptr(pointer ptr, condition_type_ur dltr) noexcept;
		unique_ptr(unique_ptr&& other) noexcept;

		template<typename OType, typename ODeleter>
		unique_ptr& operator =(unique_ptr<OType, ODeleter>&& other) noexcept;
		unique_ptr& operator =(unique_ptr&& other) noexcept;
		unique_ptr& operator =(nullptr_t) noexcept;

		typename std::add_lvalue_reference<element_type>::type operator *() const
		{
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
		unique_ptr& operator=(const unique_ptr&) = delete;

		pointer m_ptr;
		Deleter m_dltr;
	};

}

namespace simstd {

	template<typename Type, typename Deleter>
	unique_ptr<Type, Deleter>::~unique_ptr() noexcept
	{
		if (get() != pointer())
			get_deleter()(get());
		m_ptr = pointer();
	}

	template<typename Type, typename Deleter>
	constexpr unique_ptr<Type, Deleter>::unique_ptr() noexcept
		: m_ptr()
		, m_dltr(deleter_type())
	{
		static_assert(!std::is_pointer<deleter_type>::value, "constructed with null function pointer deleter");
	}

	template<typename Type, typename Deleter>
	constexpr unique_ptr<Type, Deleter>::unique_ptr(nullptr_t) noexcept
		: unique_ptr()
	{
	}

	template<typename Type, typename Deleter>
	unique_ptr<Type, Deleter>::unique_ptr(pointer ptr) noexcept
		: m_ptr(ptr)
		, m_dltr(deleter_type())
	{
		static_assert(!std::is_pointer<deleter_type>::value, "constructed with null function pointer deleter");
	}

	template<typename Type, typename Deleter>
//		template<typename _Up, typename _Ep, typename = std::_Require<std::is_convertible<typename unique_ptr<_Up, _Ep>::pointer, pointer>,std::__not_<std::is_array<_Up>>, typename std::conditional<std::is_reference<Deleter>::value, std::is_same<_Ep, Deleter>, std::is_convertible<_Ep, Deleter>>::type>>
	template<typename OType, typename ODeleter>
	unique_ptr<Type, Deleter>::unique_ptr(unique_ptr<OType, ODeleter> && other) noexcept
		: m_ptr(other.release())
		, m_dltr(simstd::forward<ODeleter>(other.get_deleter()))
	{
	}

	template<typename Type, typename Deleter>
	unique_ptr<Type, Deleter>::unique_ptr(pointer ptr, condition_type_r dltr) noexcept
		: m_ptr(ptr)
		, m_dltr(dltr)
	{
	}

	template<typename Type, typename Deleter>
	unique_ptr<Type, Deleter>::unique_ptr(pointer ptr, condition_type_ur dltr) noexcept
		: m_ptr(ptr)
		, m_dltr(simstd::move(dltr))
	{
		static_assert(!std::is_reference<deleter_type>::value, "rvalue deleter bound to reference");
	}

	template<typename Type, typename Deleter>
	unique_ptr<Type, Deleter>::unique_ptr(unique_ptr&& other) noexcept
		: m_ptr(other.release())
		, m_dltr(simstd::forward<deleter_type>(other.get_deleter()))
	{
	}

	template<typename Type, typename Deleter>
	template<typename OType, typename ODeleter>
	unique_ptr<Type, Deleter>& unique_ptr<Type, Deleter>::operator =(unique_ptr<OType, ODeleter> && other) noexcept
	{
		reset(other.release());
		get_deleter() = simstd::forward<ODeleter>(other.get_deleter());
		return *this;
	}
//	template<typename _Up, typename _Ep>
//	typename enable_if< __and_<is_convertible<typename unique_ptr<_Up, _Ep>::pointer, pointer>, __not_<is_array<_Up>>>::value, unique_ptr&>::type
//	operator = (unique_ptr<_Up, _Ep>&& other) noexcept
//	{
//		reset(other.release());
//		get_deleter() = std::forward<_Ep>(other.get_deleter());
//		return *this;
//	}

	template<typename Type, typename Deleter>
	unique_ptr<Type, Deleter>& unique_ptr<Type, Deleter>::operator =(unique_ptr&& other) noexcept
	{
		reset(other.release());
		get_deleter() = simstd::forward<deleter_type>(other.get_deleter());
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
		return m_dltr;
	}

	template<typename Type, typename Deleter>
	const typename unique_ptr<Type, Deleter>::deleter_type& unique_ptr<Type, Deleter>::get_deleter() const noexcept
	{
		return m_dltr;
	}

	template<typename Type, typename Deleter>
	unique_ptr<Type, Deleter>::operator bool() const noexcept
	{
		return get() == pointer() ? false : true;
	}

	template<typename Type, typename Deleter>
	typename unique_ptr<Type, Deleter>::pointer unique_ptr<Type, Deleter>::release() noexcept
	{
		pointer ptr = get();
		m_ptr = pointer();
		return ptr;
	}

	template<typename Type, typename Deleter>
	void unique_ptr<Type, Deleter>::reset(pointer ptr) noexcept
	{
		using simstd::swap;
		swap(m_ptr, ptr);
		if (ptr != pointer())
			get_deleter()(ptr);
	}

	template<typename Type, typename Deleter>
	void unique_ptr<Type, Deleter>::swap(unique_ptr& other) noexcept
	{
		using simstd::swap;
		swap(m_ptr, other.m_ptr);
		swap(m_dltr, other.m_dltr);
	}

}

namespace simstd {

	template<typename Type, typename Deleter>
	void swap(unique_ptr<Type, Deleter>& a, unique_ptr<Type, Deleter>& b) noexcept
	{
		a.swap(b);
	}

	template<typename Type, typename Deleter, typename _Up, typename _Ep>
	bool operator ==(const unique_ptr<Type, Deleter>& a, const unique_ptr<_Up, _Ep>& b)
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

	template<typename Type, typename Deleter, typename _Up, typename _Ep>
	bool operator <(const unique_ptr<Type, Deleter>& a, const unique_ptr<_Up, _Ep>& b)
	{
		typedef typename std::common_type<typename unique_ptr<Type, Deleter>::pointer, typename unique_ptr<_Up, _Ep>::pointer>::type CommonType;
		return simstd::less<CommonType>()(a.get(), b.get());
	}

	template<typename Type, typename Deleter>
	bool operator <(const unique_ptr<Type, Deleter>& a, nullptr_t)
	{
		return simstd::less<typename unique_ptr<Type, Deleter>::pointer>()(a.get(), nullptr);
	}

	template<typename Type, typename Deleter>
	bool operator <(nullptr_t, const unique_ptr<Type, Deleter>& a)
	{
		return simstd::less<typename unique_ptr<Type, Deleter>::pointer>()(nullptr, a.get());
	}

	template<typename Type, typename Deleter, typename _Up, typename _Ep>
	bool operator !=(const unique_ptr<Type, Deleter>& a, const unique_ptr<_Up, _Ep>& b)
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

	template<typename Type, typename Deleter, typename _Up, typename _Ep>
	bool operator <=(const unique_ptr<Type, Deleter>& a, const unique_ptr<_Up, _Ep>& b)
	{
		return rel_ops::operator <=(a, b);
	}

	template<typename Type, typename Deleter>
	bool operator <=(const unique_ptr<Type, Deleter>& a, nullptr_t)
	{
		return rel_ops::operator <=(a, nullptr);
	}

	template<typename Type, typename Deleter>
	bool operator <=(nullptr_t, const unique_ptr<Type, Deleter>& b)
	{
		return rel_ops::operator <=(nullptr, b);
	}

	template<typename Type, typename Deleter, typename _Up, typename _Ep>
	bool operator >(const unique_ptr<Type, Deleter>& a, const unique_ptr<_Up, _Ep>& b)
	{
		return rel_ops::operator >(a, b);
	}

	template<typename Type, typename Deleter>
	bool operator >(const unique_ptr<Type, Deleter>& a, nullptr_t)
	{
		return rel_ops::operator >(a, nullptr);
	}

	template<typename Type, typename Deleter>
	bool operator >(nullptr_t, const unique_ptr<Type, Deleter>& b)
	{
		return rel_ops::operator >(nullptr, b);
	}

	template<typename Type, typename Deleter, typename _Up, typename _Ep>
	bool operator >=(const unique_ptr<Type, Deleter>& a, const unique_ptr<_Up, _Ep>& b)
	{
		return rel_ops::operator >=(a, b);
	}

	template<typename Type, typename Deleter>
	bool operator >=(const unique_ptr<Type, Deleter>& a, nullptr_t)
	{
		return rel_ops::operator >=(a, nullptr);
	}

	template<typename Type, typename Deleter>
	bool operator >=(nullptr_t, const unique_ptr<Type, Deleter>& b)
	{
		return rel_ops::operator >=(nullptr, b);
	}

	template<class T, class ... Args>
	unique_ptr<T> make_unique(Args&&... args)
	{
		return simstd::move(unique_ptr<T>(new T(args...)));
	}

}

#endif
