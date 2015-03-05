#ifndef BASIS_MEMORY_SHARED_PTR_COUNTED_DELETER_HPP_
#define BASIS_MEMORY_SHARED_PTR_COUNTED_DELETER_HPP_

namespace simstd {
	namespace pvt {

		template<typename Ptr, typename Deleter, typename Allocator, LockPolicy LockPol>
		class counted_deleter final
			: public counted_base<LockPol>
		{
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
			Impl impl;
		};

	}

}

#endif
