#ifndef BASIS_MEMORY_SHARED_PTR_HPP_
#define BASIS_MEMORY_SHARED_PTR_HPP_

#ifdef __GXX_RTTI
#   include <typeinfo>
#endif

#include <basis/simstd/~memory/~shared_ptr/shared_ptr_counted_ptr.hpp>
#include <basis/simstd/~memory/~shared_ptr/shared_ptr_shared_count.hpp>
#include <basis/simstd/~memory/~shared_ptr/shared_ptr_shared_ptr_base.hpp>

template<typename Type>
class simstd::shared_ptr: public pvt::shared_ptr_base<Type>
{
	using this_type = shared_ptr<Type>;
	using base_type = pvt::shared_ptr_base<Type>;

public:
	~shared_ptr() noexcept = default;

	constexpr shared_ptr() noexcept = default;

	template<typename OType>
	explicit shared_ptr(OType* ptr): base_type(ptr) {}
	template<typename OType, typename Deleter>
	shared_ptr(OType* ptr, Deleter deleter): base_type(ptr, deleter) {}
	template<typename OType, typename Deleter, typename Allocator>
	shared_ptr(OType* ptr, Deleter deleter, Allocator allocator): base_type(ptr, deleter, move(allocator)) {}

	constexpr shared_ptr(nullptr_t) noexcept {}
	template<typename Deleter>
	shared_ptr(nullptr_t ptr, Deleter deleter): base_type(ptr, deleter) {}
	template<typename Deleter, typename Allocator>
	shared_ptr(nullptr_t ptr, Deleter deleter, Allocator allocator): base_type(ptr, deleter, move(allocator)) {}

	shared_ptr(const this_type&) noexcept = default;
	template<typename OType>
	shared_ptr(const shared_ptr<OType>& other, Type* ptr) noexcept: base_type(other, ptr) {}
	template<typename OType, typename = typename defstd::enable_if<defstd::is_convertible<OType*, Type*>::value>::type>
	shared_ptr(const shared_ptr<OType>& other) noexcept: base_type(other) {}

	shared_ptr(this_type&& other) noexcept: base_type(move(other)) {}
	template<typename OType, typename = typename defstd::enable_if<defstd::is_convertible<OType*, Type*>::value>::type>
	shared_ptr(shared_ptr<OType>&& other) noexcept: base_type(move(other)) {}

	template<typename OType>
	explicit shared_ptr(const weak_ptr<OType>& other): base_type(other) {}

	template<typename OType, typename Deleter>
	shared_ptr(unique_ptr<OType, Deleter>&& other): base_type(move(other)) {}

	this_type& operator =(const this_type&) noexcept = default;

	template<typename OType>
	this_type& operator =(const shared_ptr<OType>& other) noexcept
	{
		base_type::operator =(other);
		return *this;
	}

	this_type& operator =(this_type&& other) noexcept
	{
		base_type::operator =(move(other));
		return *this;
	}

	template<class OType>
	this_type& operator =(shared_ptr<OType>&& other) noexcept
	{
		base_type::operator =(move(other));
		return *this;
	}

	template<typename OType, typename Deleter>
	this_type& operator =(unique_ptr<OType, Deleter>&& other)
	{
		base_type::operator =(move(other));
		return *this;
	}

private:
	template<typename Allocator, typename... Args>
	shared_ptr(pvt::make_shared_tag tag, const Allocator& allocator, Args&&... args) : base_type(tag, allocator, forward<Args>(args)...) {}

	shared_ptr(const weak_ptr<Type>& other, nothrow_t tag): base_type(other, tag) {}

	template<typename OType, typename Allocator, typename... Args>
	friend shared_ptr<OType> allocate_shared(const Allocator& allocator, Args&&... args);

	friend class weak_ptr<Type>;
};

template<typename Type>
class simstd::weak_ptr: public pvt::weak_ptr_base<Type>
{
	using this_type = weak_ptr<Type>;
	using base_type = pvt::weak_ptr_base<Type>;

public:
	constexpr weak_ptr() noexcept = default;

	template<typename OType, typename = typename defstd::enable_if<defstd::is_convertible<OType*, Type*>::value>::type>
	weak_ptr(const weak_ptr<OType>& other) noexcept: base_type(other) {}

	template<typename OType, typename = typename defstd::enable_if<defstd::is_convertible<OType*, Type*>::value>::type>
	weak_ptr(const shared_ptr<OType>& other) noexcept: base_type(other) {}

	template<typename OType>
	this_type& operator =(const weak_ptr<OType>& other) noexcept
	{
		base_type::operator =(other);
		return *this;
	}

	template<typename OType>
	this_type& operator =(const shared_ptr<OType>& other) noexcept
	{
		base_type::operator =(other);
		return *this;
	}

	shared_ptr<Type> lock() const noexcept {return shared_ptr<Type>(*this, nothrow);}
};

template<typename Type>
class simstd::enable_shared_from_this
{
public:
	shared_ptr<Type> shared_from_this() {return shared_ptr<Type>(weak_this);}
	shared_ptr<const Type> shared_from_this() const {return shared_ptr<const Type>(weak_this);}

protected:
	~enable_shared_from_this() = default;
	constexpr enable_shared_from_this() noexcept = default;
	enable_shared_from_this(const enable_shared_from_this&) noexcept = default;
	enable_shared_from_this& operator =(const enable_shared_from_this&) noexcept = default;

private:
	template<typename OType>
	void assign(OType* ptr, const pvt::shared_count<>& refctr) const noexcept
	{
		weak_this.assign(ptr, refctr);
	}

	template<typename OType>
	friend void enable_shared_from_this_helper(const pvt::shared_count<>& refctr, const enable_shared_from_this* esft, const OType* ptr) noexcept
	{
		if (esft)
			esft->assign(const_cast<OType*>(ptr), refctr);
	}

	mutable weak_ptr<Type> weak_this;
};

namespace simstd
{
	template<typename TypeA, typename TypeB>
	bool operator ==(const shared_ptr<TypeA>& a, const shared_ptr<TypeB>& b) noexcept
	{
		return a.get() == b.get();
	}

	template<typename Type>
	bool operator ==(const shared_ptr<Type>& a, nullptr_t) noexcept
	{
		return !a;
	}

	template<typename Type>
	bool operator ==(nullptr_t, const shared_ptr<Type>& a) noexcept
	{
		return !a;
	}

	template<typename TypeA, typename TypeB>
	bool operator !=(const shared_ptr<TypeA>& a, const shared_ptr<TypeB>& b) noexcept
	{
		return rel_ops::operator !=(a, b);
	}

	template<typename Type>
	bool operator !=(const shared_ptr<Type>& a, nullptr_t) noexcept
	{
		return rel_ops::operator !=(a, nullptr);
	}

	template<typename Type>
	bool operator !=(nullptr_t, const shared_ptr<Type>& b) noexcept
	{
		return rel_ops::operator !=(nullptr, b);
	}

	template<typename TypeA, typename TypeB>
	bool operator <(const shared_ptr<TypeA>& a, const shared_ptr<TypeB>& b) noexcept
	{
		using CT = typename defstd::common_type<TypeA*, TypeB*>::type;
		return less<CT>()(a.get(), b.get());
	}

	template<typename Type>
	bool operator <(const shared_ptr<Type>& a, nullptr_t) noexcept
	{
		return less<Type*>()(a.get(), nullptr);
	}

	template<typename Type>
	bool operator <(nullptr_t, const shared_ptr<Type>& b) noexcept
	{
		return less<Type*>()(nullptr, b.get());
	}

	template<typename TypeA, typename TypeB>
	bool operator <=(const shared_ptr<TypeA>& a, const shared_ptr<TypeB>& b) noexcept
	{
		return rel_ops::operator <=(a, b);
	}

	template<typename Type>
	bool operator <=(const shared_ptr<Type>& a, nullptr_t) noexcept
	{
		return rel_ops::operator <=(a, nullptr);
	}

	template<typename Type>
	bool operator <=(nullptr_t, const shared_ptr<Type>& b) noexcept
	{
		return rel_ops::operator <=(nullptr, b);
	}

	template<typename TypeA, typename TypeB>
	bool operator >(const shared_ptr<TypeA>& a, const shared_ptr<TypeB>& b) noexcept
	{
		return rel_ops::operator >(a, b);
	}

	template<typename Type>
	bool operator >(const shared_ptr<Type>& a, nullptr_t) noexcept
	{
		return rel_ops::operator >(a, nullptr);
	}

	template<typename Type>
	bool operator >(nullptr_t, const shared_ptr<Type>& b) noexcept
	{
		return rel_ops::operator >(nullptr, b);
	}

	template<typename TypeA, typename TypeB>
	bool operator >=(const shared_ptr<TypeA>& a, const shared_ptr<TypeB>& b) noexcept
	{
		return rel_ops::operator >=(a, b);
	}

	template<typename Type>
	bool operator >=(const shared_ptr<Type>& a, nullptr_t) noexcept
	{
		return rel_ops::operator >=(a, nullptr);
	}

	template<typename Type>
	bool operator >=(nullptr_t, const shared_ptr<Type>& b) noexcept
	{
		return rel_ops::operator >=(nullptr, b);
	}

//	template<typename Type>
//	struct less<shared_ptr<Type>> : public _Sp_less<shared_ptr<Type>> {
//	};

	template<typename Type>
	void swap(shared_ptr<Type>& a, shared_ptr<Type>& b) noexcept
	{
		a.swap(b);
	}

	template<typename Type, typename OType>
	shared_ptr<Type> static_pointer_cast(const shared_ptr<OType>& other) noexcept
	{
		return shared_ptr<Type>(other, static_cast<Type*>(other.get()));
	}

	template<typename Type, typename OType>
	shared_ptr<Type> const_pointer_cast(const shared_ptr<OType>& other) noexcept
	{
		return shared_ptr<Type>(other, const_cast<Type*>(other.get()));
	}

	template<typename Type, typename OType>
	shared_ptr<Type> dynamic_pointer_cast(const shared_ptr<OType>& other) noexcept
	{
		if (Type* ptr = dynamic_cast<Type*>(other.get()))
			return shared_ptr<Type>(other, ptr);
		return shared_ptr<Type>();
	}

	template<typename Type, typename Allocator, typename... Args>
	shared_ptr<Type> allocate_shared(const Allocator& allocator, Args&&... args)
	{
		TraceFunc();
		return shared_ptr<Type>(pvt::make_shared_tag(), allocator, forward<Args>(args)...);
	}

	template<typename Type, typename... Args>
	shared_ptr<Type> make_shared(Args&&... args)
	{
		TraceFunc();
		return allocate_shared<Type>(allocator<remove_const_t<Type>>(), forward<Args>(args)...);
	}
}

#endif
