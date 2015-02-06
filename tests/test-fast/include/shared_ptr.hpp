#ifndef BASIS_MEMORY_SHARED_PTR_HPP_
#define BASIS_MEMORY_SHARED_PTR_HPP_

#include <basis/configure.hpp>
#include <basis/simstd/memory>

#include <typeinfo>

#include "~shared_ptr/forward.hpp"
#include "~shared_ptr/counted_ptr.hpp"
#include "~shared_ptr/shared_ptr.hpp"

namespace simstd1 {

	template<typename Type>
	class shared_ptr: public pvt::shared_ptr<Type>
	{
		using this_type = shared_ptr<Type>;
		using base_type = pvt::shared_ptr<Type>;

	public:
		constexpr shared_ptr() noexcept = default;

		template<typename OType>
		explicit shared_ptr(OType* ptr): pvt::shared_ptr<OType>(ptr) {}
		template<typename OType, typename Deleter>
		shared_ptr(OType* ptr, Deleter deleter): pvt::shared_ptr<OType>(ptr, deleter) {}
		template<typename OType, typename Deleter, typename Allocator>
		shared_ptr(OType* ptr, Deleter deleter, Allocator allocator): pvt::shared_ptr<OType>(ptr, deleter, simstd::move(allocator)) {}

		constexpr shared_ptr(nullptr_t) noexcept {};
		template<typename Deleter>
		shared_ptr(nullptr_t ptr, Deleter deleter): base_type(ptr, deleter) {}
		template<typename Deleter, typename Allocator>
		shared_ptr(nullptr_t ptr, Deleter deleter, Allocator allocator): base_type(ptr, deleter, simstd::move(allocator)) {}

		shared_ptr(const this_type&) noexcept = default;
		template<typename OType, typename = typename std::enable_if<std::is_convertible<OType*, Type*>::value>::type>
		shared_ptr(const shared_ptr<OType>& other) noexcept: base_type(other) {}

		shared_ptr(this_type&& other) noexcept: base_type(simstd::move(other)) {}
		template<typename OType, typename = typename std::enable_if<std::is_convertible<OType*, Type*>::value>::type>
		shared_ptr(shared_ptr<OType>&& other) noexcept: base_type(simstd::move(other)) {}

		template<typename OType>
		explicit shared_ptr(const weak_ptr<OType>& other): base_type(other) {}

		template<typename OType, typename Deleter>
		shared_ptr(simstd::unique_ptr<OType, Deleter>&& other): base_type(simstd::move(other)) {}

		this_type& operator =(const this_type&) noexcept = default;

		template<typename OType>
		this_type& operator =(const shared_ptr<OType>& other) noexcept
		{
			this->base_type::operator =(other);
			return *this;
		}

		this_type& operator =(this_type&& other) noexcept
		{
			this->base_type::operator =(simstd::move(other));
			return *this;
		}

		template<class OType>
		this_type& operator =(shared_ptr<OType>&& other) noexcept
		{
			this->base_type::operator =(simstd::move(other));
			return *this;
		}

		template<typename OType, typename Deleter>
		this_type& operator=(simstd::unique_ptr<OType, Deleter>&& other)
		{
			this->base_type::operator =(simstd::move(other));
			return *this;
		}

	private:
		template<typename Allocator, typename ... Args>
		shared_ptr(pvt::make_shared_tag tag, const Allocator& allocator, Args&&... args)
			: base_type(tag, allocator, simstd::forward<Args>(args)...)
		{
		}

		template<typename OType, typename Allocator, typename ... Args>
		friend shared_ptr<OType> allocate_shared(const Allocator& allocator, Args&&... args);

		shared_ptr(const weak_ptr<Type>& other, simstd::nothrow_t tag): base_type(other, tag) {}

		friend class weak_ptr<Type>;
	};

	template<typename Type>
	class weak_ptr: public pvt::weak_ptr<Type>
	{
		using this_type = weak_ptr<Type>;
		using base_type = pvt::weak_ptr<Type>;

	public:
		constexpr weak_ptr() noexcept = default;

		template<typename OType, typename = typename std::enable_if<std::is_convertible<OType*, Type*>::value>::type>
		weak_ptr(const weak_ptr<OType>& other) noexcept: base_type(other) {}

		template<typename OType, typename = typename std::enable_if<std::is_convertible<OType*, Type*>::value>::type>
		weak_ptr(const shared_ptr<OType>& other) noexcept: base_type(other) {}

		template<typename OType>
		this_type& operator =(const weak_ptr<OType>& other) noexcept
		{
			this->base_type::operator =(other);
			return *this;
		}

		template<typename OType>
		this_type&
		operator=(const shared_ptr<OType>& other) noexcept
		{
			this->base_type::operator =(other);
			return *this;
		}

		shared_ptr<Type> lock() const noexcept
		{
			return shared_ptr<Type>(*this, simstd::nothrow);
		}

	};

	template<typename Type>
	class enable_shared_from_this
	{
	public:
		shared_ptr<Type> shared_from_this()
		{
			return shared_ptr<Type>(this->_M_weak_this);
		}

		shared_ptr<const Type> shared_from_this() const
		{
			return shared_ptr<const Type>(this->_M_weak_this);
		}

	protected:
		~enable_shared_from_this() = default;
		constexpr enable_shared_from_this() noexcept = default;
		enable_shared_from_this(const enable_shared_from_this&) noexcept = default;
		enable_shared_from_this& operator =(const enable_shared_from_this&) noexcept = default;

	private:
		template<typename OType>
		void _weak_assign(OType* ptr, const pvt::shared_count<>& __n) const noexcept
		{
			_M_weak_this._M_assign(ptr, __n);
		}

		template<typename OType>
		friend void __enable_shared_from_this_helper(const pvt::shared_count<>& __pn, const enable_shared_from_this* __pe, const OType* px) noexcept
		{
			if (__pe != 0)
				__pe->_weak_assign(const_cast<OType*>(px), __pn);
		}

		mutable weak_ptr<Type> _M_weak_this;
	};

	template<typename Type, typename Allocator, typename... Args>
	shared_ptr<Type> allocate_shared(const Allocator& allocator, Args&&... args)
	{
		return shared_ptr<Type>(pvt::make_shared_tag(), allocator, simstd::forward<Args>(args)...);
	}

	template<typename Type, typename... Args>
	shared_ptr<Type> make_shared(Args&&... args)
	{
		typedef typename std::remove_const<Type>::type Type_nc;
		return allocate_shared<Type>(simstd::allocator<Type_nc>(), simstd::forward<Args>(args)...);
	}

}

#endif
