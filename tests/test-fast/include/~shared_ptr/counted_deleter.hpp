#ifndef BASIS_MEMORY_SHARED_PTR_COUNTED_DELETER_HPP_
#define BASIS_MEMORY_SHARED_PTR_COUNTED_DELETER_HPP_

#include "counted_base.hpp"

namespace simstd1 {

	namespace pvt {

		template<typename Ptr, typename Deleter, typename Allocator, LockPolicy LockPol>
		class counted_deleter final : public counted_base<LockPol>
		{
			class Impl: private ebo_helper<0, Deleter>, private ebo_helper<1, Allocator>
			{
				using Deleter_base = ebo_helper<0, Deleter>;
				using Allocator_base = ebo_helper<1, Allocator>;

			public:
				Impl(Ptr ptr, Deleter deleter, const Allocator& allocator) noexcept : ptr(ptr), Deleter_base(deleter), Allocator_base(allocator) {}
				Deleter& get_deleter() noexcept {return Deleter_base::get(*this);}
				Allocator& get_allocator() noexcept {return Allocator_base::get(*this);}
				Ptr ptr;
			};

		public:
			counted_deleter(Ptr ptr, Deleter deleter) noexcept: impl(ptr, deleter, Allocator()) {}

			counted_deleter(Ptr ptr, Deleter deleter, const Allocator& allocator) noexcept: impl(ptr, deleter, allocator) {}

			void dispose() noexcept {impl.get_deleter()(impl.ptr);}

			void destroy() noexcept
			{
				using Alloc_traits = typename simstd::allocator_traits<Allocator>::template rebind_traits<counted_deleter>;
//				typename _Alloc_traits::allocator_type __a(impl.get_allocator());
				auto allocator(impl.get_allocator());
				Alloc_traits::destroy(allocator, this);
				Alloc_traits::deallocate(allocator, this, 1);
			}

			void* get_deleter(const std::type_info& ti) noexcept
			{
#ifdef __GXX_RTTI
				return ti == typeid(Deleter) ? &impl.get_deleter() : nullptr;
#else
				return nullptr;
#endif
			}

		private:
			Impl impl;
		};

	}

}

#endif
