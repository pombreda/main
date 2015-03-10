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
				: ptr()
			{
			}

			template<typename OType>
			explicit shared_ptr_base(OType* ptr)
				: refctr(ptr)
				, ptr(ptr)
			{
//				__glibcxx_function_requires(_ConvertibleConcept<OType*, Type*>)
				static_assert(!simstd::is_void<OType>::value, "incomplete type");
				static_assert(sizeof(OType) > 0, "incomplete type");
				enable_shared_from_this_helper(refctr, ptr, ptr);
			}

			template<typename OType, typename Deleter>
			shared_ptr_base(OType* ptr, Deleter deleter)
				: refctr(ptr, deleter)
				, ptr(ptr)
			{
//				__glibcxx_function_requires(_ConvertibleConcept<OType*, Type*>)
				enable_shared_from_this_helper(refctr, ptr, ptr);
			}

			template<typename OType, typename Deleter, typename Allocator>
			shared_ptr_base(OType* ptr, Deleter deleter, Allocator allocator)
				: refctr(ptr, deleter, simstd::move(allocator))
				, ptr(ptr)
			{
//				__glibcxx_function_requires(_ConvertibleConcept<OType*, Type*>)
				enable_shared_from_this_helper(refctr, ptr, ptr);
			}

			template<typename Deleter>
			shared_ptr_base(nullptr_t ptr, Deleter deleter)
				: refctr(ptr, deleter)
				, ptr()
			{
			}

			template<typename Deleter, typename Allocator>
			shared_ptr_base(nullptr_t ptr, Deleter deleter, Allocator allocator)
				: refctr(ptr, deleter, simstd::move(allocator))
				, ptr()
			{
			}

			shared_ptr_base(const shared_ptr_base&) noexcept = default;

			template<typename OType>
			shared_ptr_base(const shared_ptr_base<OType, LockPol>& other, Type* ptr) noexcept
				: refctr(other.refctr)
				, ptr(ptr)
			{
			}

			template<typename OType, typename = typename defstd::enable_if<defstd::is_convertible<OType*, Type*>::value>::type>
			shared_ptr_base(const shared_ptr_base<OType, LockPol>& other) noexcept
				: refctr(other.refctr)
				, ptr(other.ptr)
			{
			}

			shared_ptr_base(shared_ptr_base&& other) noexcept
				: ptr(other.ptr)
			{
				refctr.swap(other.refctr);
				other.ptr = nullptr;
			}

			template<typename OType, typename = typename defstd::enable_if<defstd::is_convertible<OType*, Type*>::value>::type>
			shared_ptr_base(shared_ptr_base<OType, LockPol>&& other) noexcept
				: ptr(other.ptr)
			{
				refctr.swap(other.refctr);
				other.ptr = nullptr;
			}

			template<typename OType>
			explicit shared_ptr_base(const weak_ptr_base<OType, LockPol>& other)
				: refctr(other.refctr, simstd::nothrow)
				, ptr(other.ptr)
			{
			}

			template<typename OType, typename Deleter>
			shared_ptr_base(simstd::unique_ptr<OType, Deleter>&& other)
				: ptr(other.get())
			{
				auto __raw = raw_ptr(other.get());
				refctr = shared_count<LockPol>(simstd::move(other));
				enable_shared_from_this_helper(refctr, __raw, __raw);
			}

			constexpr shared_ptr_base(nullptr_t) noexcept
				: refctr()
				, ptr()
			{
			}

			shared_ptr_base& operator =(const shared_ptr_base& other) noexcept = default;

			template<typename OType>
			shared_ptr_base& operator =(const shared_ptr_base<OType, LockPol>& other) noexcept
			{
				refctr = other.refctr;
				ptr = other.ptr;
				return *this;
			}

			shared_ptr_base& operator =(shared_ptr_base&& other) noexcept
			{
				shared_ptr_base(simstd::move(other)).swap(*this);
				return *this;
			}

			template<class OType>
			shared_ptr_base& operator =(shared_ptr_base<OType, LockPol>&& other) noexcept
			{
				shared_ptr_base(simstd::move(other)).swap(*this);
				return *this;
			}

			template<typename OType, typename Deleter>
			shared_ptr_base& operator =(simstd::unique_ptr<OType, Deleter>&& other)
			{
				shared_ptr_base(simstd::move(other)).swap(*this);
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

			template<typename OType, typename Deleter>
			void reset(OType* ptr, Deleter deleter)
			{
				shared_ptr_base(ptr, deleter).swap(*this);
			}

			template<typename OType, typename Deleter, typename Allocator>
			void reset(OType* ptr, Deleter deleter, Allocator allocator)
			{
				shared_ptr_base(ptr, deleter, simstd::move(allocator)).swap(*this);
			}

			// Allow class instantiation when Type is [cv-qual] void.
			typename defstd::add_lvalue_reference<Type>::type operator*() const noexcept
			{
				CRT_ASSERT(ptr);
				return *ptr;
			}

			Type* operator ->() const noexcept
			{
				CRT_ASSERT(ptr);
				return ptr;
			}

			Type* get() const noexcept
			{
				return ptr;
			}

			explicit operator bool() const // never throws
			{
				return ptr ? true : false;
			}

			bool unique() const noexcept
			{
				return refctr.unique();
			}

			ssize_t use_count() const noexcept
			{
				return refctr.get_use_count();
			}

			void swap(shared_ptr_base<Type, LockPol>& other) noexcept
			{
				using simstd::swap;
				refctr.swap(other.refctr);
				swap(ptr, other.ptr);
			}

			template<typename OType>
			bool owner_before(const shared_ptr_base<OType, LockPol>& other) const
			{
				return refctr.less(other.refctr);
			}

			template<typename OType>
			bool owner_before(const weak_ptr_base<OType, LockPol>& other) const
			{
				return refctr.less(other.refctr);
			}

#ifdef __GXX_RTTI
		protected:
			// This constructor is non-standard, it is used by allocate_shared.
			template<typename Allocator, typename... Args>
			shared_ptr_base(make_shared_tag tag, const Allocator& allocator, Args&&... args)
				: refctr(tag, (Type*)nullptr, allocator, simstd::forward<Args>(args)...)
				, ptr()
			{
				// ptr needs to point to the newly constructed object.
				// This relies on _Sp_counted_ptr_inplace::_M_get_deleter.
				void* __p = refctr.get_deleter(typeid(tag));
				ptr = static_cast<Type*>(__p);
				enable_shared_from_this_helper(refctr, ptr, ptr);
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
				: refctr()
				, ptr()
			{
				typedef typename _Alloc::template rebind<Type>::other _Alloc2;
				_Deleter<_Alloc2> __del = {_Alloc2(__a)};
				typedef simstd::allocator_traits<_Alloc2> __traits;
				ptr = __traits::allocate(__del._M_alloc, 1);
				__traits::construct(__del._M_alloc, ptr, simstd::forward<_Args>(__args)...);
				shared_count<LockPol> __count(ptr, __del, __del._M_alloc);
				refctr.swap(__count);
				enable_shared_from_this_helper(refctr, ptr, ptr);
			}
#endif

			template<typename OType, LockPolicy OLockPol, typename _Alloc, typename... _Args>
			friend shared_ptr_base<OType, OLockPol> allocate_shared(const _Alloc& __a, _Args&&... __args);

			// This constructor is used by weak_ptr::lock() and shared_ptr::shared_ptr(const weak_ptr&, defstd::nothrow_t).
			shared_ptr_base(const weak_ptr_base<Type, LockPol>& other, simstd::nothrow_t)
				: refctr(other.refctr, simstd::nothrow)
			{
				ptr = refctr.get_use_count() ? other.ptr : nullptr;
			}

			friend class weak_ptr_base<Type, LockPol>;

		private:
			void* get_deleter(const defstd::type_info& __ti) const noexcept {return refctr.get_deleter(__ti);}

			template<typename OType>
			static OType* raw_ptr(OType* ptr) {return ptr;}

			template<typename OType>
			static auto raw_ptr(OType notptr) -> decltype(simstd::addressof(*notptr)) {return simstd::addressof(*notptr);}

			template<typename OType, LockPolicy OLockPol> friend class shared_ptr_base;
			template<typename OType, LockPolicy OLockPol> friend class weak_ptr_base;

			template<typename Deleter, typename OType, LockPolicy OLockPol>
			friend Deleter* simstd::get_deleter(const pvt::shared_ptr_base<OType, OLockPol>& sptr) noexcept;

			shared_count<LockPol> refctr;
			element_type*         ptr;
		};


		template<typename Type, LockPolicy LockPol>
		class weak_ptr_base
		{
			using this_type = weak_ptr_base;

		public:
			using element_type = Type;

			constexpr weak_ptr_base() noexcept = default;

			weak_ptr_base(const weak_ptr_base&) noexcept = default;
			weak_ptr_base& operator =(const weak_ptr_base&) noexcept = default;

			// The "obvious" converting constructor implementation:
			//
			//  template<typename OType>
			//  weak_ptr(const weak_ptr<OType, LockPol>& other)
			//      : refctr(other.refctr), ptr(other.ptr)  // never throws
			//  {}
			//
			// has a serious problem.
			//
			//  other.ptr may already have been invalidated. The ptr(other.ptr)
			//  conversion may require access to *other.ptr (virtual inheritance).
			//
			// It is not possible to avoid spurious access violations since
			// in multithreaded programs other.ptr may be invalidated at any point.
			template<typename OType, typename = typename defstd::enable_if<defstd::is_convertible<OType*, Type*>::value>::type>
			weak_ptr_base(const weak_ptr_base<OType, LockPol>& other) noexcept
				: refctr(other.refctr)
			{
				ptr = other.lock().get(); // here temporary shared_ptr will be created
			}

			template<typename OType, typename = typename defstd::enable_if<defstd::is_convertible<OType*, Type*>::value>::type>
			weak_ptr_base(const shared_ptr_base<OType, LockPol>& other) noexcept
				: refctr(other.refctr)
				, ptr(other.ptr)
			{
			}

			template<typename OType>
			weak_ptr_base& operator =(const weak_ptr_base<OType, LockPol>& other) noexcept
			{
				refctr = other.refctr;
				ptr = other.lock().get();
				return *this;
			}

			template<typename OType>
			weak_ptr_base& operator =(const shared_ptr_base<OType, LockPol>& other) noexcept
			{
				refctr = other.refctr;
				ptr = other.ptr;
				return *this;
			}

			shared_ptr_base<Type, LockPol> lock() const noexcept
			{
				return shared_ptr_base<element_type, LockPol>(*this, simstd::nothrow);
			}

			ssize_t use_count() const noexcept
			{
				return refctr.get_use_count();
			}

			bool expired() const noexcept
			{
				return refctr.get_use_count() == 0;
			}

			template<typename OType>
			bool owner_before(const shared_ptr_base<OType, LockPol>& other) const
			{
				return refctr.less(other.refctr);
			}

			template<typename OType>
			bool owner_before(const weak_ptr_base<OType, LockPol>& other) const
			{
				return refctr.less(other.refctr);
			}

			void reset() noexcept
			{
				weak_ptr_base().swap(*this);
			}

			void swap(weak_ptr_base& other) noexcept
			{
				using simstd::swap;
				refctr.swap(other.refctr);
				swap(ptr, other.ptr);
			}

		private:
			// Used by enable_shared_from_this.
			void assign(Type* ptr, const shared_count<LockPol>& refctr) noexcept
			{
				this->refctr = refctr;
				this->ptr = ptr;
			}

			template<typename OType, LockPolicy OLockPol> friend class shared_ptr_base;
			template<typename OType, LockPolicy OLockPol> friend class weak_ptr_base;

			friend class enable_shared_from_this<Type, LockPol>;
			friend class simstd::enable_shared_from_this<Type>;

			weak_count<LockPol> refctr;
			Type*               ptr = nullptr;
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
			shared_ptr_base<Type, LockPol> shared_from_this() {return shared_ptr_base<Type, LockPol>(weak_base);}
			shared_ptr_base<const Type, LockPol> shared_from_this() const {return shared_ptr_base<const Type, LockPol>(weak_base);}

		protected:
			~enable_shared_from_this() = default;
			constexpr enable_shared_from_this() noexcept = default;
			enable_shared_from_this(const enable_shared_from_this&) noexcept = default;
			enable_shared_from_this& operator =(const enable_shared_from_this&) noexcept = default;

		private:
			template<typename OType>
			void assign(OType* ptr, const shared_count<LockPol>& refctr) const noexcept {weak_base.assign(ptr, refctr);}

			template<typename OType>
			friend void enable_shared_from_this_helper(const shared_count<LockPol>& refctr, const enable_shared_from_this* esft, const OType* ptr) noexcept
			{
				if (esft)
					esft->assign(const_cast<OType*>(ptr), refctr);
			}

			mutable weak_ptr_base<Type, LockPol> weak_base;
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
