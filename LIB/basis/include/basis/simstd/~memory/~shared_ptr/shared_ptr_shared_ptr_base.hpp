#ifndef BASIS_MEMORY_SHARED_PTR_SHARED_PTR_HPP_
#define BASIS_MEMORY_SHARED_PTR_SHARED_PTR_HPP_

namespace simstd {

	// Friend of enable_shared_from_this.
	template<typename Type, typename OType>
	void enable_shared_from_this_helper(const pvt::shared_count<>&, const enable_shared_from_this<Type>*, const OType*) noexcept;

	template<typename Deleter, typename Type, pvt::LockPolicy OLockPol>
	Deleter* get_deleter(const pvt::shared_ptr_base<Type, OLockPol>&) noexcept;

	namespace pvt {

		// Friend of enable_shared_from_this.
		template<LockPolicy LockPol, typename Type, typename OType>
		void enable_shared_from_this_helper(const shared_count<LockPol>&, const enable_shared_from_this<Type, LockPol>*, const OType*) noexcept;

		template<LockPolicy LockPol>
		void enable_shared_from_this_helper(const shared_count<LockPol>&, ...) noexcept {}

		template<typename Type, LockPolicy LockPol>
		class shared_ptr_base {
		public:
			using element_type = Type;

			constexpr shared_ptr_base() noexcept
				: _M_ptr(nullptr)
			{
			}

			template<typename OType>
			explicit shared_ptr_base(OType* ptr)
				: _M_refcount(ptr)
				, _M_ptr(ptr)
			{
//				__glibcxx_function_requires(_ConvertibleConcept<OType*, Type*>)
				static_assert(!std::is_void<OType>::value, "incomplete type" );
				static_assert(sizeof(OType) > 0, "incomplete type" );
				enable_shared_from_this_helper(_M_refcount, ptr, ptr);
			}

			template<typename OType, typename Deleter>
			shared_ptr_base(OType* ptr, Deleter deleter)
				: _M_refcount(ptr, deleter)
				, _M_ptr(ptr)
			{
//				__glibcxx_function_requires(_ConvertibleConcept<OType*, Type*>)
				enable_shared_from_this_helper(_M_refcount, ptr, ptr);
			}

			template<typename OType, typename Deleter, typename Allocator>
			shared_ptr_base(OType* ptr, Deleter deleter, Allocator allocator)
				: _M_refcount(ptr, deleter, simstd::move(allocator))
				, _M_ptr(ptr)
			{
//				__glibcxx_function_requires(_ConvertibleConcept<OType*, Type*>)
				enable_shared_from_this_helper(_M_refcount, ptr, ptr);
			}

			template<typename Deleter>
			shared_ptr_base(nullptr_t ptr, Deleter deleter)
				: _M_refcount(ptr, deleter)
				, _M_ptr(0)
			{
			}

			template<typename Deleter, typename Allocator>
			shared_ptr_base(nullptr_t ptr, Deleter deleter, Allocator allocator)
				: _M_refcount(ptr, deleter, simstd::move(allocator))
				, _M_ptr(0)
			{
			}

			shared_ptr_base(const shared_ptr_base&) noexcept = default;

			template<typename OType>
			shared_ptr_base(const shared_ptr_base<OType, LockPol>& other, Type* ptr) noexcept
				: _M_refcount(other._M_refcount)
				, _M_ptr(ptr)
			{
			}

			template<typename OType, typename = typename std::enable_if<std::is_convertible<OType*, Type*>::value>::type>
			shared_ptr_base(const shared_ptr_base<OType, LockPol>& other) noexcept
				: _M_refcount(other._M_refcount)
				, _M_ptr(other._M_ptr)
			{
			}

			shared_ptr_base(shared_ptr_base&& other) noexcept
				: _M_ptr(other._M_ptr)
			{
				_M_refcount.swap(other._M_refcount);
				other._M_ptr = nullptr;
			}

			template<typename OType, typename = typename std::enable_if<std::is_convertible<OType*, Type*>::value>::type>
			shared_ptr_base(shared_ptr_base<OType, LockPol>&& other) noexcept
				: _M_ptr(other._M_ptr)
			{
				_M_refcount.swap(other._M_refcount);
				other._M_ptr = nullptr;
			}

			template<typename OType>
			explicit shared_ptr_base(const weak_ptr_base<OType, LockPol>& other)
				: _M_refcount(other._M_refcount, simstd::nothrow)
				, _M_ptr(other._M_ptr)
			{
			}

			template<typename OType, typename Deleter>
			shared_ptr_base(simstd::unique_ptr<OType, Deleter>&& other)
				: _M_ptr(other.get())
			{
//				__glibcxx_function_requires(_ConvertibleConcept<OType*, Type*>)
				auto __raw = raw_ptr(other.get());
				_M_refcount = shared_count<LockPol>(simstd::move(other));
				enable_shared_from_this_helper(_M_refcount, __raw, __raw);
			}

			constexpr shared_ptr_base(nullptr_t) noexcept
				: _M_refcount()
				, _M_ptr(0)
			{
			}

			shared_ptr_base& operator =(const shared_ptr_base&) noexcept = default;

			template<typename OType>
			shared_ptr_base& operator =(const shared_ptr_base<OType, LockPol>& __r) noexcept
			{
				_M_refcount = __r._M_refcount; // __shared_count::op= doesn't throw
				_M_ptr = __r._M_ptr;
				return *this;
			}

			shared_ptr_base& operator =(shared_ptr_base&& __r) noexcept
			{
				shared_ptr_base(simstd::move(__r)).swap(*this);
				return *this;
			}

			template<class OType>
			shared_ptr_base& operator =(shared_ptr_base<OType, LockPol>&& __r) noexcept
			{
				shared_ptr_base(simstd::move(__r)).swap(*this);
				return *this;
			}

			template<typename OType, typename Deleter>
			shared_ptr_base& operator =(simstd::unique_ptr<OType, Deleter>&& __r)
			{
				shared_ptr_base(simstd::move(__r)).swap(*this);
				return *this;
			}

			void reset() noexcept
			{
				shared_ptr_base().swap(*this);
			}

			template<typename OType>
			void reset(OType* ptr)
			{
				shared_ptr_base(ptr).swap(*this);
			}

		template<typename OType, typename _Deleter>
		void
		reset(OType* __p, _Deleter __d)
		{	shared_ptr_base(__p, __d).swap(*this);}

		template<typename OType, typename _Deleter, typename _Alloc>
		void reset(OType* __p, _Deleter __d, _Alloc __a) {shared_ptr_base(__p, __d, simstd::move(__a)).swap(*this);}

		// Allow class instantiation when Type is [cv-qual] void.
		typename std::add_lvalue_reference<Type>::type
		operator*() const noexcept
		{
//			_GLIBCXX_DEBUG_ASSERT(_M_ptr != 0);
			return *_M_ptr;
		}

		Type* operator->() const noexcept
		{
			CRT_ASSERT(_M_ptr);
			return _M_ptr;
		}

		Type* get() const noexcept
		{
			return _M_ptr;
		}

		explicit operator bool() const // never throws
		{	return _M_ptr == 0 ? false : true;}

		bool unique() const noexcept
		{
			return _M_refcount.unique();
		}

		ssize_t use_count() const noexcept
		{
			return _M_refcount.get_use_count();
		}

		void swap(shared_ptr_base<Type, LockPol>& __other) noexcept
		{
			using simstd::swap;
			swap(_M_ptr, __other._M_ptr);
			_M_refcount.swap(__other._M_refcount);
		}

		template<typename OType>
		bool owner_before(shared_ptr_base<OType, LockPol> const& __rhs) const
		{	return _M_refcount._M_less(__rhs._M_refcount);}

		template<typename OType>
		bool owner_before(weak_ptr_base<OType, LockPol> const& __rhs) const {return _M_refcount.less(__rhs._M_refcount);}

#ifdef __GXX_RTTI
	protected:
		// This constructor is non-standard, it is used by allocate_shared.
		template<typename Allocator, typename... Args>
		shared_ptr_base(make_shared_tag tag, const Allocator& allocator, Args&&... args)
			: _M_refcount(tag, (Type*)nullptr, allocator, simstd::forward<Args>(args)...)
			, _M_ptr()
		{
			TraceFunc();
			// _M_ptr needs to point to the newly constructed object.
			// This relies on _Sp_counted_ptr_inplace::_M_get_deleter.
			void* __p = _M_refcount.get_deleter(typeid(tag));
			_M_ptr = static_cast<Type*>(__p);
			enable_shared_from_this_helper(_M_refcount, _M_ptr, _M_ptr);
		}
#else
		template<typename Allocator>
		struct _Deleter
		{
			void operator()(Type* __ptr)
			{
				typedef simstd::allocator_traits<Allocator> _Alloc_traits;
				_Alloc_traits::destroy(_M_alloc, __ptr);
				_Alloc_traits::deallocate(_M_alloc, __ptr, 1);
			}
			Allocator _M_alloc;
		};

		template<typename _Alloc, typename... _Args>
		shared_ptr_base(make_shared_tag, const _Alloc& __a, _Args&&... __args)
			: _M_refcount()
			, _M_ptr()
		{
			typedef typename _Alloc::template rebind<Type>::other _Alloc2;
			_Deleter<_Alloc2> __del = {_Alloc2(__a)};
			typedef simstd::allocator_traits<_Alloc2> __traits;
			_M_ptr = __traits::allocate(__del._M_alloc, 1);
			__traits::construct(__del._M_alloc, _M_ptr, simstd::forward<_Args>(__args)...);
			shared_count<LockPol> __count(_M_ptr, __del, __del._M_alloc);
			_M_refcount.swap(__count);
			enable_shared_from_this_helper(_M_refcount, _M_ptr, _M_ptr);
		}
#endif

			template<typename OType, LockPolicy OLockPol, typename _Alloc, typename... _Args>
			friend shared_ptr_base<OType, OLockPol> allocate_shared(const _Alloc& __a, _Args&&... __args);

			// This constructor is used by weak_ptr::lock() and shared_ptr::shared_ptr(const weak_ptr&, std::nothrow_t).
			shared_ptr_base(const weak_ptr_base<Type, LockPol>& other, simstd::nothrow_t)
				: _M_refcount(other._M_refcount, simstd::nothrow)
			{
				_M_ptr = _M_refcount.get_use_count() ? other._M_ptr : nullptr;
			}

			friend class weak_ptr_base<Type, LockPol>;

		private:
			void* get_deleter(const std::type_info& __ti) const noexcept {return _M_refcount.get_deleter(__ti);}

			template<typename OType>
			static OType* raw_ptr(OType* ptr) {return ptr;}

			template<typename OType>
			static auto raw_ptr(OType __ptr) -> decltype(simstd::addressof(*__ptr)) {return simstd::addressof(*__ptr);}

			template<typename OType, LockPolicy OLockPol> friend class shared_ptr_base;
			template<typename OType, LockPolicy OLockPol> friend class weak_ptr_base;

			template<typename Deleter, typename OType, LockPolicy OLockPol>
			friend Deleter* simstd::get_deleter(const pvt::shared_ptr_base<OType, OLockPol>& sptr) noexcept;

			shared_count<LockPol> _M_refcount;
			element_type*         _M_ptr;
		};


		template<typename Type, LockPolicy LockPol>
		class weak_ptr_base
		{
			using this_type = weak_ptr_base;

		public:
			typedef Type element_type;

			constexpr weak_ptr_base() noexcept
				: _M_refcount()
				, _M_ptr(nullptr)
			{
			}

			weak_ptr_base(const weak_ptr_base&) noexcept = default;
			weak_ptr_base& operator =(const weak_ptr_base&) noexcept = default;

			// The "obvious" converting constructor implementation:
			//
			//  template<typename OType>
			//    weak_ptr(const weak_ptr<OType, LockPol>& __r)
			//    : _M_ptr(__r._M_ptr), _M_refcount(__r._M_refcount) // never throws
			//    { }
			//
			// has a serious problem.
			//
			//  __r._M_ptr may already have been invalidated. The _M_ptr(__r._M_ptr)
			//  conversion may require access to *__r._M_ptr (virtual inheritance).
			//
			// It is not possible to avoid spurious access violations since
			// in multithreaded programs __r._M_ptr may be invalidated at any point.
			template<typename OType, typename = typename std::enable_if<std::is_convertible<OType*, Type*>::value>::type>
			weak_ptr_base(const weak_ptr_base<OType, LockPol>& other) noexcept
				: _M_refcount(other._M_refcount)
			{
				_M_ptr = other.lock().get();
			}

			template<typename OType, typename = typename std::enable_if<std::is_convertible<OType*, Type*>::value>::type>
			weak_ptr_base(const shared_ptr_base<OType, LockPol>& other) noexcept
				: _M_refcount(other._M_refcount)
				, _M_ptr(other._M_ptr)
			{
			}

			template<typename OType>
			weak_ptr_base& operator =(const weak_ptr_base<OType, LockPol>& other) noexcept
			{
				_M_ptr = other.lock().get();
				_M_refcount = other._M_refcount;
				return *this;
			}

			template<typename OType>
			weak_ptr_base& operator =(const shared_ptr_base<OType, LockPol>& other) noexcept
			{
				_M_ptr = other._M_ptr;
				_M_refcount = other._M_refcount;
				return *this;
			}

			shared_ptr_base<Type, LockPol> lock() const noexcept
			{
				return shared_ptr_base<element_type, LockPol>(*this, simstd::nothrow);
			}

			ssize_t use_count() const noexcept
			{
				return _M_refcount.get_use_count();
			}

			bool expired() const noexcept
			{
				return _M_refcount.get_use_count() == 0;
			}

			template<typename OType>
			bool owner_before(const shared_ptr_base<OType, LockPol>& other) const
			{
				return _M_refcount._M_less(other._M_refcount);
			}

			template<typename OType>
			bool owner_before(const weak_ptr_base<OType, LockPol>& other) const
			{
				return _M_refcount._M_less(other._M_refcount);
			}

			void reset() noexcept
			{
				weak_ptr_base().swap(*this);
			}

			void swap(weak_ptr_base& other) noexcept
			{
				using simstd::swap;
				swap(_M_ptr, other._M_ptr);
				_M_refcount._M_swap(other._M_refcount);
			}

		private:
			// Used by enable_shared_from_this.
			void _M_assign(Type* ptr, const shared_count<LockPol>& refcount) noexcept
			{
				this->_M_ptr = ptr;
				this->_M_refcount = refcount;
			}

			template<typename OType, LockPolicy _Lp1> friend class shared_ptr_base;
			template<typename OType, LockPolicy _Lp1> friend class weak_ptr_base;

			friend class enable_shared_from_this<Type, LockPol>;
			friend class simstd::enable_shared_from_this<Type>;

			weak_count<LockPol> _M_refcount;
			Type*               _M_ptr;
		};

		template<typename Type, LockPolicy LockPol>
		void swap(weak_ptr_base<Type, LockPol>& left, weak_ptr_base<Type, LockPol>& right) noexcept
		{
			left.swap(right);
		}

		template<typename Type, LockPolicy LockPol>
		class enable_shared_from_this
		{
		public:
			shared_ptr_base<Type, LockPol> shared_from_this() {return shared_ptr_base<Type, LockPol>(this->_M_weak_this);}

			shared_ptr_base<const Type, LockPol> shared_from_this() const {return shared_ptr_base<const Type, LockPol>(this->_M_weak_this);}

		protected:
			~enable_shared_from_this() {}

			constexpr enable_shared_from_this() noexcept {}

			enable_shared_from_this(const enable_shared_from_this&) noexcept {}

			enable_shared_from_this& operator =(const enable_shared_from_this&) noexcept {return *this;}

		private:
			template<typename OType>
			void _M_weak_assign(OType* __p, const shared_count<LockPol>& __n) const noexcept {_M_weak_this._M_assign(__p, __n);}

			template<typename OType>
			friend void enable_shared_from_this_helper(const shared_count<LockPol>& __pn, const enable_shared_from_this* __pe, const OType* __px) noexcept
			{
				if (__pe != 0)
					__pe->_M_weak_assign(const_cast<OType*>(__px), __pn);
			}

			mutable weak_ptr_base<Type, LockPol> _M_weak_this;
		};

	}

	template<typename Deleter, typename Type, pvt::LockPolicy LockPol>
	Deleter* get_deleter(const pvt::shared_ptr_base<Type, LockPol>& sptr) noexcept
	{
#ifdef __GXX_RTTI
		return static_cast<Deleter*>(sptr.get_deleter(typeid(Deleter)));
#else
		UNUSED(sptr);
		return 0;
#endif
	}

}

#endif
