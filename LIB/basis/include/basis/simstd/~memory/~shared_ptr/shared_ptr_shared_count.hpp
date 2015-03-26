#ifndef BASIS_MEMORY_SHARED_PTR_SHARED_COUNT_HPP_
#define BASIS_MEMORY_SHARED_PTR_SHARED_COUNT_HPP_

template<simstd::pvt::LockPolicy LockPol>
class simstd::pvt::shared_count
{
public:
	~shared_count() noexcept
	{
		if (counted)
			counted->release();
	}

	constexpr shared_count() noexcept
		: counted()
	{
	}

	template<typename Ptr>
	explicit shared_count(Ptr ptr)
		: counted(new counted_ptr<Ptr, LockPol>(ptr))
	{
		CRT_ASSERT(counted);
	}

	template<typename Ptr, typename Deleter>
	shared_count(Ptr ptr, Deleter deleter)
		: shared_count(ptr, simstd::move(deleter), simstd::allocator<void>())
	{
	}

	template<typename Ptr, typename Deleter, typename Allocator>
	shared_count(Ptr ptr, Deleter deleter, Allocator allocator)
		: counted()
	{
		using counted_type = counted_deleter<Ptr, Deleter, Allocator, LockPol>;
		using allocator_traits = typename simstd::allocator_traits<Allocator>::template rebind_traits<counted_type>;

		typename allocator_traits::allocator_type l_allocator(allocator);
		counted_type* tmp = allocator_traits::allocate(l_allocator, 1);
		CRT_ASSERT(tmp);

		allocator_traits::construct(l_allocator, tmp, ptr, simstd::move(deleter), simstd::move(allocator));
		counted = tmp;
	}

	template<typename Type, typename Allocator, typename... Args>
	shared_count(make_shared_tag, Type*, const Allocator& allocator, Args&&... args)
		: counted()
	{
		using counted_type = counted_ptr_inplace<Type, Allocator, LockPol>;
		using allocator_traits = typename simstd::allocator_traits<Allocator>::template rebind_traits<counted_type>;

		typename allocator_traits::allocator_type l_allocator(allocator);
		counted_type* tmp = allocator_traits::allocate(l_allocator, 1);
		CRT_ASSERT(tmp);

		allocator_traits::construct(l_allocator, tmp, simstd::move(allocator), simstd::forward<Args>(args)...);
		counted = tmp;
	}

	template<typename Type, typename Deleter>
	explicit shared_count(simstd::unique_ptr<Type, Deleter>&& other)
		: shared_count(other.release(), other.get_deleter(), simstd::allocator<void>())
	{
	}

	explicit shared_count(const weak_count<LockPol>& other, simstd::nothrow_t)
		: counted(other.counted)
	{
		if (counted && !counted->add_use_ref_count_check())
			counted = nullptr;
	}

	shared_count(const shared_count& other) noexcept
		: counted(other.counted)
	{
		if (counted)
			counted->add_use_ref_count_copy();
	}

	shared_count& operator =(const shared_count& other) noexcept
	{
		if (counted != other.counted)
			shared_count(other).swap(*this);
		return *this;
	}

	void swap(shared_count& other) noexcept
	{
		using simstd::swap;
		swap(counted, other.counted);
	}

	ssize_t get_use_count() const noexcept {return counted ? counted->get_use_count() : 0;}

	bool unique() const noexcept {return get_use_count() == 1;}

	void* get_deleter(const defstd::type_info& ti) const noexcept {return counted ? counted->get_deleter(ti) : nullptr;}

	bool less(const shared_count& other) const noexcept {return counted < other.counted;}

	bool less(const weak_count<LockPol>& other) const noexcept {return counted < other.counted;}

	friend bool operator ==(const shared_count& a, const shared_count& b) noexcept {return a.counted == b.counted;}

private:
	counted_base<LockPol>* counted;

	friend class weak_count<LockPol>;
};

template<simstd::pvt::LockPolicy LockPol>
class simstd::pvt::weak_count
{
public:
	~weak_count() noexcept
	{
		if (counted)
			counted->weak_release();
	}

	constexpr weak_count() noexcept
		: counted()
	{
	}

	weak_count(const weak_count& other) noexcept
		: counted(other.counted)
	{
		if (counted)
			counted->add_weak_ref_count();
	}

	weak_count(const shared_count<LockPol>& other) noexcept
		: counted(other.counted)
	{
		if (counted)
			counted->add_weak_ref_count();
	}

	weak_count& operator =(const weak_count& other) noexcept
	{
		if (counted != other.counted)
			weak_count(other).swap(*this);
		return *this;
	}

	weak_count& operator =(const shared_count<LockPol>& other) noexcept
	{
		if (counted != other.counted)
			weak_count(other).swap(*this);
		return *this;
	}

	void swap(weak_count& other) noexcept
	{
		using simstd::swap;
		swap(counted, other.counted);
	}

	ssize_t get_use_count() const noexcept {return counted ? counted->get_use_count() : 0;}

	bool less(const weak_count& other) const noexcept {return counted < other.counted;}

	bool less(const shared_count<LockPol>& other) const noexcept {return counted < other.counted;}

	friend bool operator ==(const weak_count& a, const weak_count& b) noexcept {return a.counted == b.counted;}

private:
	counted_base<LockPol>* counted;

	friend class shared_count<LockPol>;
};

#endif
