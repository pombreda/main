#ifndef BASIS_LIST_BASE_HPP_
#define BASIS_LIST_BASE_HPP_

namespace simstd {

	namespace pvt {

		template<typename Type, typename TypeAllocator>
		class List_base {
		protected:
			using node_allocator_type = typename TypeAllocator::template rebind<List_node<Type>>::other;

		public:
			using allocator_type = TypeAllocator;

			List_base();

			List_base(const node_allocator_type& allocator) ;

			List_base(List_base && other);

		protected:
			node_allocator_type& get_node_allocator();

			const node_allocator_type& get_node_allocator() const;

			template<typename ... Args>
			List_node<Type>* new_node(Args &&... args);

			void delete_node(const List_node_base* ptr);

			struct List_impl: public node_allocator_type {
				List_impl();
				List_impl(const node_allocator_type& other);
				List_impl(node_allocator_type&& other);
				List_node_base m_end;
			};

			List_impl m_impl;

		private:
			List_node<Type>* alloc_node();

			void free_node(List_node<Type>* ptr);
		};

		template<typename Type, typename Allocator>
		List_base<Type, Allocator>::List_impl::List_impl() :
			node_allocator_type()
		{
		}

		template<typename Type, typename Allocator>
		List_base<Type, Allocator>::List_impl::List_impl(const node_allocator_type& other) :
			node_allocator_type(other)
		{
		}

		template<typename Type, typename Allocator>
		List_base<Type, Allocator>::List_impl::List_impl(node_allocator_type&& other) :
			node_allocator_type(simstd::move(other))
		{
		}

		template<typename Type, typename Allocator>
		List_base<Type, Allocator>::List_base() :
			m_impl()
		{
		}

		template<typename Type, typename Allocator>
		List_base<Type, Allocator>::List_base(const node_allocator_type& allocator) :
			m_impl(allocator)
		{
		}

		template<typename Type, typename Allocator>
		List_base<Type, Allocator>::List_base(List_base && other) :
			m_impl(simstd::move(other.get_node_allocator()))
		{
			List_node_base::swap(m_impl.m_end, other.m_impl.m_end);
		}

		template<typename Type, typename Allocator>
		typename List_base<Type, Allocator>::node_allocator_type& List_base<Type, Allocator>::get_node_allocator()
		{
			return *static_cast<node_allocator_type*>(&m_impl);
		}

		template<typename Type, typename Allocator>
		const typename List_base<Type, Allocator>::node_allocator_type& List_base<Type, Allocator>::get_node_allocator() const
		{
			return *static_cast<const node_allocator_type*>(&m_impl);
		}

		template<typename Type, typename Allocator>
		template<typename ... Args>
		List_node<Type>* List_base<Type, Allocator>::new_node(Args&&... args)
		{
			auto ptr = alloc_node();
			get_node_allocator().construct(ptr, simstd::forward<Args>(args)...);
			return ptr;
		}

		template<typename Type, typename Allocator>
		void List_base<Type, Allocator>::delete_node(const List_node_base* ptr)
		{
			auto node = static_cast<List_node<Type>*>(const_cast<List_node_base*>(ptr));
			get_node_allocator().destroy(node);
			free_node(node);
		}

		template<typename Type, typename Allocator>
		List_node<Type>* List_base<Type, Allocator>::alloc_node()
		{
			return m_impl.node_allocator_type::allocate(1);
		}

		template<typename Type, typename Allocator>
		void List_base<Type, Allocator>::free_node(List_node<Type>* p)
		{
			m_impl.node_allocator_type::deallocate(p, 1);
		}

	}
}

#endif
