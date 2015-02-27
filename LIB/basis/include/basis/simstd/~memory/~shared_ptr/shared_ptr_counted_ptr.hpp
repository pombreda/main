#ifndef BASIS_MEMORY_SHARED_PTR_COUNTED_PTR_HPP_
#define BASIS_MEMORY_SHARED_PTR_COUNTED_PTR_HPP_

namespace simstd {

	namespace pvt {

		template<typename Ptr, LockPolicy LockPol>
		class counted_ptr final : public counted_base<LockPol> {
		public:
			explicit counted_ptr(Ptr ptr) noexcept : ptr(ptr)
			{
				static_assert(std::is_pointer<Ptr>::value, "constructed with nonpointer");
			}

			void dispose() noexcept {delete ptr;}

			void destroy() noexcept {delete this;}

			void* get_deleter(const std::type_info&) noexcept {return nullptr;}

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
		inline void counted_ptr<nullptr_t, LockPolicy::NONE>::dispose() noexcept
		{
		}

		template<>
		inline void counted_ptr<nullptr_t, LockPolicy::MUTEX>::dispose() noexcept
		{
		}

	}

}

#endif
