#ifndef BASIS_MEMORY_SHARED_PTR_SHARED_COUNT_HPP_
#define BASIS_MEMORY_SHARED_PTR_SHARED_COUNT_HPP_

namespace simstd1 {

	namespace pvt {

		template<LockPolicy LockPol>
		class shared_count
		{
		public:
			~shared_count() noexcept
			{
				if (_M_pi)
					_M_pi->release();
			}

			constexpr shared_count() noexcept
				: _M_pi(nullptr)
			{
			}

			template<typename Ptr>
			explicit shared_count(Ptr ptr)
				: _M_pi(new counted_ptr<Ptr, LockPol>(ptr))
			{
			}

			template<typename Ptr, typename Deleter>
			shared_count(Ptr ptr, Deleter deleter)
				: shared_count(ptr, simstd::move(deleter), simstd::allocator<void>())
			{}

			template<typename Ptr, typename Deleter, typename Allocator>
			shared_count(Ptr ptr, Deleter deleter, Allocator allocator)
				: _M_pi(nullptr)
			{
				using _Sp_cd_type = counted_deleter<Ptr, Deleter, Allocator, LockPol>;
				using _Alloc_traits = typename simstd::allocator_traits<Allocator>::template rebind_traits<_Sp_cd_type>;
				auto l_alloc(allocator);

				auto __mem = _Alloc_traits::allocate(l_alloc, 1);
				if (__mem) {
					_Alloc_traits::construct(l_alloc, __mem, ptr, simstd::move(deleter), simstd::move(allocator));
					_M_pi = __mem;
				}
			}

			template<typename Type, typename Allocator, typename... Args>
			shared_count(make_shared_tag, Type*, const Allocator& allocator, Args&&... args)
				: _M_pi(nullptr)
			{
				typedef counted_ptr_inplace<Type, Allocator, LockPol> _Sp_cp_type;
				typedef typename simstd::allocator_traits<Allocator>::template rebind_traits<_Sp_cp_type> _Alloc_traits;

				auto l_allocator(allocator);
				_Sp_cp_type* __mem = _Alloc_traits::allocate(l_allocator, 1);
				if (__mem)
				{
					_Alloc_traits::construct(l_allocator, __mem, simstd::move(allocator), simstd::forward<Args>(args)...);
					_M_pi = __mem;
				}
			}

			template<typename Type, typename Deleter>
			explicit shared_count(simstd::unique_ptr<Type, Deleter>&& other)
				: _M_pi(nullptr)
			{
				using Ptr = typename simstd::unique_ptr<Type, Deleter>::pointer;
				using Del2 = typename std::conditional<std::is_reference<Deleter>::value, std::reference_wrapper<typename std::remove_reference<Deleter>::type>, Deleter>::type;
				using Sp_cd_type = counted_deleter<Ptr, Del2, simstd::allocator<void>, LockPol>;
				using Allocator = simstd::allocator<Sp_cd_type>;
				using Alloc_traits = simstd::allocator_traits<Allocator>;

				Allocator allocator;
				Sp_cd_type* __mem = Alloc_traits::allocate(allocator, 1);
				Alloc_traits::construct(allocator, __mem, other.release(), other.get_deleter());
				_M_pi = __mem;
			}

			explicit shared_count(const weak_count<LockPol>& other, simstd::nothrow_t);

			shared_count(const shared_count& other) noexcept
				: _M_pi(other._M_pi)
			{
				if (_M_pi)
					_M_pi->use_add_ref();
			}

			shared_count& operator =(const shared_count& other) noexcept
			{
				if (_M_pi != other._M_pi)
					shared_count(other).swap(*this);
				return *this;
			}

			void swap(shared_count& other) noexcept
			{
				using simstd::swap;
				swap(_M_pi, other._M_pi);
			}

			ssize_t get_use_count() const noexcept {return _M_pi ? _M_pi->get_use_count() : 0;}

			bool unique() const noexcept {return get_use_count() == 1;}

			void* get_deleter(const std::type_info& ti) const noexcept {return _M_pi ? _M_pi->get_deleter(ti) : nullptr;}

			bool less(const shared_count& other) const noexcept {return _M_pi < other._M_pi;}

			bool less(const weak_count<LockPol>& other) const noexcept {return _M_pi < other._M_pi;}

			friend bool operator ==(const shared_count& a, const shared_count& b) noexcept {return a._M_pi == b._M_pi;}

		private:
			counted_base<LockPol>* _M_pi;

			friend class weak_count<LockPol>;
		};

		template<LockPolicy LockPol>
		class weak_count
		{
		public:
			~weak_count() noexcept
			{
				if (_M_pi)
					_M_pi->weak_release();
			}

			constexpr weak_count() noexcept: _M_pi(0) {}

			weak_count(const shared_count<LockPol>& other) noexcept
				: _M_pi(other._M_pi)
			{
				if (_M_pi)
					_M_pi->weak_add_ref();
			}

			weak_count(const weak_count<LockPol>& other) noexcept
				: _M_pi(other._M_pi)
			{
				if (_M_pi)
					_M_pi->weak_add_ref();
			}

			weak_count<LockPol>& operator =(const shared_count<LockPol>& other) noexcept
			{
				if (_M_pi != other._M_pi)
					weak_count(other).swap(*this);
				return *this;
			}

			weak_count<LockPol>& operator =(const weak_count<LockPol>& other) noexcept
			{
				if (_M_pi != other._M_pi)
					weak_count(other).swap(*this);
				return *this;
			}

			void swap(weak_count<LockPol>& other) noexcept
			{
				using simstd::swap;
				sawp(_M_pi, other._M_pi);
			}

			long get_use_count() const noexcept {return _M_pi ? _M_pi->get_use_count() : 0;}

			bool less(const weak_count& other) const noexcept {return _M_pi < other._M_pi;}

			bool less(const shared_count<LockPol>& other) const noexcept {return _M_pi < other._M_pi;}

			friend bool operator ==(const weak_count& a, const weak_count& b) noexcept {return a._M_pi == b._M_pi;}

		private:
			friend class shared_count<LockPol>;

			counted_base<LockPol>* _M_pi;
		};

		template<LockPolicy LockPol>
		shared_count<LockPol>::shared_count(const weak_count<LockPol>& other, simstd::nothrow_t)
			: _M_pi(other._M_pi)
		{
//			CRT_ASSERT(_M_pi);
			if (_M_pi != nullptr)
				if (!_M_pi->use_add_ref())
					_M_pi = nullptr;
		}
	}
}

#endif
