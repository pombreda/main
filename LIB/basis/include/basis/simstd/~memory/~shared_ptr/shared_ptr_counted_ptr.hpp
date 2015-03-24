#ifndef BASIS_MEMORY_SHARED_PTR_COUNTED_PTR_HPP_
#define BASIS_MEMORY_SHARED_PTR_COUNTED_PTR_HPP_

namespace simstd
{
	namespace pvt
	{
		template<LockPolicy LockPol>
		struct choose_lock_policy
		{
			ssize_t get_use_count() const noexcept {return use_count;}
			ssize_t get_weak_count() const noexcept {return weak_count;}
			void add_use_ref_count_copy() noexcept {++use_count;}
			bool add_use_ref_count_check() noexcept {return use_count == 0 ? false : ++use_count, true;}
			void add_weak_ref_count() noexcept {++weak_count;}

		protected:
			choose_lock_policy(ssize_t use_count, ssize_t weak_count) noexcept: use_count(use_count), weak_count(weak_count) {}
			bool sub_use_ref_count() noexcept {return use_count-- == 1;}
			bool sub_weak_ref_count() noexcept {return weak_count-- == 1;}

		private:
			ssize_t use_count;
			ssize_t weak_count;
		};

		template<>
		struct choose_lock_policy<LockPolicy::ATOMIC>
		{
			ssize_t get_use_count() const noexcept {return use_count;}
			ssize_t get_weak_count() const noexcept {return weak_count;}
			void add_use_ref_count_copy() noexcept {++use_count;}
			bool add_use_ref_count_check() noexcept {return use_count.add_if_not_equal(1, 0);}
			void add_weak_ref_count() noexcept {++weak_count;}

		protected:
			choose_lock_policy(ssize_t use_count, ssize_t weak_count) noexcept: use_count(use_count), weak_count(weak_count) {}
			bool sub_use_ref_count() noexcept {return use_count-- == 1;}
			bool sub_weak_ref_count() noexcept {return weak_count-- == 1;}

		private:
			atomic_int use_count;
			atomic_int weak_count;
		};

		template<>
		struct choose_lock_policy<LockPolicy::MUTEX>
		{
			ssize_t get_use_count() const noexcept {return use_count;}
			ssize_t get_weak_count() const noexcept {return weak_count;}
			void add_use_ref_count_copy() noexcept {increase_use();}
			bool add_use_ref_count_check() noexcept {return add_if_not_equal(use_count, 1, 0);}
			void add_weak_ref_count() noexcept {increase_weak();}

		protected:
			~choose_lock_policy() noexcept {DeleteCriticalSection(&mutex);}
			choose_lock_policy(ssize_t use_count, ssize_t weak_count) noexcept: use_count(use_count), weak_count(weak_count) {InitializeCriticalSection(&mutex);}
			bool sub_use_ref_count() noexcept {return decrease_use() == 1;}
			bool sub_weak_ref_count() noexcept {return decrease_weak() == 1;}

		private:
			void increase_use() {EnterCriticalSection(&mutex); ++use_count; LeaveCriticalSection(&mutex);}
			ssize_t decrease_use() {EnterCriticalSection(&mutex); ssize_t ret = use_count--; LeaveCriticalSection(&mutex); return ret;}
			void increase_weak() {EnterCriticalSection(&mutex); ++weak_count; LeaveCriticalSection(&mutex);}
			ssize_t decrease_weak() {EnterCriticalSection(&mutex); ssize_t ret = weak_count--; LeaveCriticalSection(&mutex); return ret;}
			bool add_if_not_equal(ssize_t& value, ssize_t addition, ssize_t compare)
			{
				bool ret = false;
				EnterCriticalSection(&mutex);
				if (value != compare)
				{
					value += addition;
					ret = true;
				}
				LeaveCriticalSection(&mutex);
				return ret;
			}

			ssize_t use_count;
			ssize_t weak_count;
			CRITICAL_SECTION mutex;
		};

		template<LockPolicy LockPol = DEFAULT_LOCK_POLICY>
		class counted_base: public choose_lock_policy<LockPol>
		{
		public:
			virtual ~counted_base() noexcept = default;

			counted_base() noexcept
				: choose_lock_policy<LockPol>(1, 1)
			{
			}

			void release() noexcept
			{
				if (choose_lock_policy<LockPol>::sub_use_ref_count())
				{
					dispose();
					weak_release();
				}
			}

			void weak_release() noexcept
			{
				if (choose_lock_policy<LockPol>::sub_weak_ref_count())
					destroy();
			}

			virtual void* get_deleter(const defstd::type_info&) noexcept = 0;

		private:
			virtual void dispose() noexcept = 0;

			virtual void destroy() noexcept = 0;

			counted_base(const counted_base&) = delete;
			counted_base& operator =(const counted_base&) = delete;
		};
	}
}

namespace simstd
{
	namespace pvt
	{
		template<typename Ptr, LockPolicy LockPol>
		class counted_ptr final
			: public counted_base<LockPol>
		{
		public:
			explicit counted_ptr(Ptr ptr) noexcept :
				ptr(ptr)
			{
				static_assert(simstd::is_pointer<Ptr>::value, "constructed with nonpointer");
			}

			void dispose() noexcept {delete ptr;}

			void destroy() noexcept {delete this;}

			void* get_deleter(const defstd::type_info&) noexcept {return nullptr;}

		private:
			counted_ptr(const counted_ptr&) = delete;
			counted_ptr& operator =(const counted_ptr&) = delete;

			Ptr ptr;
		};

		template<>
		inline void counted_ptr<nullptr_t, LockPolicy::ATOMIC>::dispose() noexcept
		{
		}

		template<>
		inline void counted_ptr<nullptr_t, LockPolicy::MUTEX>::dispose() noexcept
		{
		}

		template<>
		inline void counted_ptr<nullptr_t, LockPolicy::NONE>::dispose() noexcept
		{
		}

		template<typename Type, typename Allocator, LockPolicy LockPol>
		class counted_ptr_inplace final
			: public counted_base<LockPol>
		{
		public:
			template<typename... Args>
			counted_ptr_inplace(Allocator allocator, Args&&... args)
				: impl(allocator)
			{
				simstd::allocator_traits<Allocator>::construct(allocator, get_ptr(), simstd::forward<Args>(args)...);
			}

			void dispose() noexcept
			{
				simstd::allocator_traits<Allocator>::destroy(impl.get_allocator(), get_ptr());
			}

			void destroy() noexcept
			{
				using Alloc_traits = typename simstd::allocator_traits<Allocator>::template rebind_traits<counted_ptr_inplace>;
				typename Alloc_traits::allocator_type allocator(impl.get_allocator());
				Alloc_traits::destroy(allocator, this);
				Alloc_traits::deallocate(allocator, this, 1);
			}

			void* get_deleter(const defstd::type_info& ti) noexcept
			{
#ifdef __GXX_RTTI
				if (ti == typeid(make_shared_tag))
					return const_cast<typename simstd::remove_cv<Type>::type*>(get_ptr());
#endif
				return nullptr;
			}

		private:
			Type* get_ptr() noexcept {return impl.storage.ptr();}

			struct Impl: private simstd::pvt::ebo_helper<0, Allocator>
			{
				using base_type = simstd::pvt::ebo_helper<0, Allocator>;
				explicit Impl(Allocator allocator) noexcept: base_type(allocator) {}
				Allocator& get_allocator() noexcept {return base_type::get(*this);}
				aligned_buffer<Type> storage;
			};

			Impl impl;
		};

		template<typename Ptr, typename Deleter, typename Allocator, LockPolicy LockPol>
		class counted_deleter final
			: public counted_base<LockPol>
		{
		public:
			counted_deleter(Ptr ptr, const Deleter& deleter) noexcept: impl(ptr, deleter, Allocator()) {}

			counted_deleter(Ptr ptr, const Deleter& deleter, const Allocator& allocator) noexcept: impl(ptr, deleter, allocator) {}

			void dispose() noexcept
			{
				impl.get_deleter()(impl.ptr);
			}

			void destroy() noexcept
			{
				using Alloc_traits = typename simstd::allocator_traits<Allocator>::template rebind_traits<counted_deleter>;
				typename Alloc_traits::allocator_type allocator(impl.get_allocator());
				Alloc_traits::destroy(allocator, this);
				Alloc_traits::deallocate(allocator, this, 1);
			}

			void* get_deleter(const defstd::type_info& ti) noexcept
			{
#ifdef __GXX_RTTI
				return ti == typeid(Deleter) ? &impl.get_deleter() : nullptr;
#else
				UNUSED(ti);
				return nullptr;
#endif
			}

		private:
			class Impl
				: private simstd::pvt::ebo_helper<0, Deleter>
				, private simstd::pvt::ebo_helper<1, Allocator>
			{
				using deleter_base_type = simstd::pvt::ebo_helper<0, Deleter>;
				using allocator_base_type = simstd::pvt::ebo_helper<1, Allocator>;

			public:
				Impl(Ptr ptr, const Deleter& deleter, const Allocator& allocator) noexcept : deleter_base_type(deleter), allocator_base_type(allocator), ptr(ptr) {}
				Deleter& get_deleter() noexcept {return deleter_base_type::get(*this);}
				Allocator& get_allocator() noexcept {return allocator_base_type::get(*this);}
				Ptr ptr;
			};

			Impl impl;
		};
	}
}

#endif
