#ifndef BASIS_MEMORY_SHARED_PTR_COUNTED_PTR_INPLACE_HPP_
#define BASIS_MEMORY_SHARED_PTR_COUNTED_PTR_INPLACE_HPP_

namespace simstd {

	namespace pvt {

		template<typename Type, typename Allocator, LockPolicy LockPol>
		class counted_ptr_inplace final : public counted_base<LockPol>
		{
			struct Impl: private simstd::pvt::ebo_helper<0, Allocator>
			{
				using base_type = simstd::pvt::ebo_helper<0, Allocator>;
				explicit Impl(Allocator allocator) noexcept: base_type(allocator) {}
				Allocator& get_allocator() noexcept {return base_type::get(*this);}
				aligned_buffer<Type> storage;
			};

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

			Impl impl;
		};

	}

}

#endif
