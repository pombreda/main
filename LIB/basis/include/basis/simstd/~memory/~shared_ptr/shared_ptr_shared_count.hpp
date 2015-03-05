#ifndef BASIS_MEMORY_SHARED_PTR_SHARED_COUNT_HPP_
#define BASIS_MEMORY_SHARED_PTR_SHARED_COUNT_HPP_

namespace simstd
{
	namespace pvt
	{
		template<LockPolicy LockPol>
		class shared_count
		{
		public:
			~shared_count() noexcept
			{
				if (counter)
					counter->release();
			}

			constexpr shared_count() noexcept
				: counter()
			{
			}

			template<typename Ptr>
			explicit shared_count(Ptr ptr)
				: counter(new counted_ptr<Ptr, LockPol>(ptr))
			{
			}

			template<typename Ptr, typename Deleter>
			shared_count(Ptr ptr, Deleter deleter)
				: shared_count(ptr, simstd::move(deleter), simstd::allocator<void>())
			{
			}

			template<typename Ptr, typename Deleter, typename Allocator>
			shared_count(Ptr ptr, Deleter deleter, Allocator allocator)
				: counter()
			{
				using counter_type = counted_deleter<Ptr, Deleter, Allocator, LockPol>;
				using allocator_traits = typename simstd::allocator_traits<Allocator>::template rebind_traits<counter_type>;

				typename allocator_traits::allocator_type l_allocator(allocator);
				auto l_counter = allocator_traits::allocate(l_allocator, 1);
				CRT_ASSERT(l_counter);

				allocator_traits::construct(l_allocator, l_counter, ptr, simstd::move(deleter), simstd::move(allocator));
				counter = l_counter;
			}

			template<typename Type, typename Allocator, typename... Args>
			shared_count(make_shared_tag, Type*, const Allocator& allocator, Args&&... args)
				: counter()
			{
				TraceFunc();
				using counter_type = counted_ptr_inplace<Type, Allocator, LockPol>;
				using allocator_traits = typename simstd::allocator_traits<Allocator>::template rebind_traits<counter_type>;

				typename allocator_traits::allocator_type l_allocator(allocator);
				auto l_counter = allocator_traits::allocate(l_allocator, 1);
				CRT_ASSERT(l_counter);

				allocator_traits::construct(l_allocator, l_counter, simstd::move(allocator), simstd::forward<Args>(args)...);
				counter = l_counter;
			}

			template<typename Type, typename Deleter>
			explicit shared_count(simstd::unique_ptr<Type, Deleter>&& other)
				: shared_count(other.release(), other.get_deleter(), simstd::allocator<void>())
//				: counter()
			{
//				using OPtr = typename simstd::unique_ptr<Type, Deleter>::pointer;
//				using ODeleter = typename defstd::conditional<defstd::is_reference<Deleter>::value, defstd::reference_wrapper<typename defstd::remove_reference<Deleter>::type>, Deleter>::type;
//				using counter_type = counted_deleter<OPtr, ODeleter, simstd::allocator<void>, LockPol>;
//				using allocator_traits = simstd::allocator_traits<simstd::allocator<counter_type>>;
//
//				typename allocator_traits::allocator_type l_allocator;
//				auto l_counter = allocator_traits::allocate(l_allocator, 1);
//				CRT_ASSERT(l_counter);
//
//				allocator_traits::construct(l_allocator, l_counter, other.release(), other.get_deleter());
//				counter = l_counter;
			}

			explicit shared_count(const weak_count<LockPol>& other, simstd::nothrow_t)
				: counter(other.counter)
			{
				if (counter)
					if (!counter->add_use_ref_count())
						counter = nullptr;
			}

			shared_count(const shared_count& other) noexcept
				: counter(other.counter)
			{
				if (counter)
					counter->add_use_ref_count();
			}

			shared_count& operator =(const shared_count& other) noexcept
			{
				if (counter != other.counter)
					shared_count(other).swap(*this);
				return *this;
			}

			void swap(shared_count& other) noexcept
			{
				using simstd::swap;
				swap(counter, other.counter);
			}

			ssize_t get_use_count() const noexcept {return counter ? counter->get_use_count() : 0;}

			bool unique() const noexcept {return get_use_count() == 1;}

			void* get_deleter(const defstd::type_info& ti) const noexcept {return counter ? counter->get_deleter(ti) : nullptr;}

			bool less(const shared_count& other) const noexcept {return counter < other.counter;}

			bool less(const weak_count<LockPol>& other) const noexcept {return counter < other.counter;}

			friend bool operator ==(const shared_count& a, const shared_count& b) noexcept {return a.counter == b.counter;}

		private:
			counted_base<LockPol>* counter;

			friend class weak_count<LockPol>;
		};

		template<LockPolicy LockPol>
		class weak_count
		{
		public:
			~weak_count() noexcept
			{
				if (counter)
					counter->weak_release();
			}

			constexpr weak_count() noexcept
				: counter()
			{
			}

			weak_count(const shared_count<LockPol>& other) noexcept
				: counter(other.counter)
			{
				if (counter)
					counter->add_weak_ref_count();
			}

			weak_count(const weak_count<LockPol>& other) noexcept
				: counter(other.counter)
			{
				if (counter)
					counter->add_weak_ref_count();
			}

			weak_count<LockPol>& operator =(const shared_count<LockPol>& other) noexcept
			{
				if (counter != other.counter)
					weak_count(other).swap(*this);
				return *this;
			}

			weak_count<LockPol>& operator =(const weak_count<LockPol>& other) noexcept
			{
				if (counter != other.counter)
					weak_count(other).swap(*this);
				return *this;
			}

			void swap(weak_count<LockPol>& other) noexcept
			{
				using simstd::swap;
				swap(counter, other.counter);
			}

			ssize_t get_use_count() const noexcept {return counter ? counter->get_use_count() : 0;}

			bool less(const weak_count& other) const noexcept {return counter < other.counter;}

			bool less(const shared_count<LockPol>& other) const noexcept {return counter < other.counter;}

			friend bool operator ==(const weak_count& a, const weak_count& b) noexcept {return a.counter == b.counter;}

		private:
			counted_base<LockPol>* counter;

			friend class shared_count<LockPol>;
		};

	}
}

#endif
