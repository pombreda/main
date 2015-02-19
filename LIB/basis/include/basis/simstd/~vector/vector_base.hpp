#ifndef BASIS_SIMSTD_VECTOR_VECTOR_BASE_HPP_
#define BASIS_SIMSTD_VECTOR_VECTOR_BASE_HPP_

namespace simstd {

//		template<typename Type, typename Allocator>
//		class vector_base
//		{
//			typedef vector_base this_type;
//
//		public:
//			typedef Type value_type;
//			typedef Allocator allocator_type;
//			typedef allocator_traits<allocator_type> traits_type;
//			typedef typename traits_type::size_type size_type;
//			typedef typename traits_type::pointer pointer;
//
//			allocator_type allocator;
//			pointer begin = nullptr;
//			pointer end = nullptr;
//			pointer end_of_storage = nullptr;
//
//			~vector_base();
//			vector_base(const allocator_type& alloc);
//			vector_base(size_type capa, const allocator_type& alloc);
//			vector_base(size_type capa, const this_type& other);
//			vector_base(size_type capa, const this_type& other, const allocator_type& alloc);
//			vector_base(this_type&& other);
//			vector_base(this_type&& other, const allocator_type& alloc);
//			void swap(this_type& other);
//
//			template<typename ... Args>
//			void construct(pointer ptr, Args&&... args);
//
//			void destroy(pointer first, pointer last);
//			void reserve(size_type newCapacity);
//			void clear();
//			void adjust_capacity(size_type addToSize);
//			bool check_capacity_if_size_grows(size_type addToSize) const;
//			size_type get_new_capacity(size_type addToSize) const;
//			size_type capacity() const;
//			size_type size() const;
//			size_type max_size() const;
//
//		private:
//			void create_storage(size_type capa);
//		};
//
//		template<typename Type, typename Allocator>
//		vector_base<Type, Allocator>::~vector_base()
//		{
//			if (begin) {
//				clear();
//				traits_type::deallocate(allocator, begin, capacity());
//			}
//		}
//
//		template<typename Type, typename Allocator>
//		vector_base<Type, Allocator>::vector_base(const allocator_type& alloc)
//			: allocator(alloc)
//		{
//		}
//
//		template<typename Type, typename Allocator>
//		vector_base<Type, Allocator>::vector_base(size_type capa, const allocator_type& alloc)
//			: allocator(alloc)
//		{
//			create_storage(capa);
//		}
//
//		template<typename Type, typename Allocator>
//		vector_base<Type, Allocator>::vector_base(size_type capa, const this_type& other)
//			: allocator(other.allocator)
//		{
//			create_storage(simstd::max(capa, other.size()));
//			end = simstd::uninitialized_copy(other.begin, other.end, end);
//		}
//
//		template<typename Type, typename Allocator>
//		vector_base<Type, Allocator>::vector_base(size_type capa, const this_type& other, const allocator_type& alloc)
//			: allocator(alloc)
//		{
//			create_storage(simstd::max(capa, other.size()));
//			end = simstd::uninitialized_copy(other.begin, other.end, end);
//		}
//
//		template<typename Type, typename Allocator>
//		vector_base<Type, Allocator>::vector_base(this_type&& other)
//			: allocator(simstd::move(other.allocator))
//		{
//			swap(other);
//		}
//
//		template<typename Type, typename Allocator>
//		vector_base<Type, Allocator>::vector_base(this_type&& other, const allocator_type& alloc)
//			: allocator(alloc)
//		{
//			if (allocator == other.allocator) {
//				swap(other);
//			} else {
//				create_storage(other.size());
//				end = simstd::uninitialized_copy(simstd::make_move_iterator(other.begin), simstd::make_move_iterator(other.end), end);
//				other.clear();
//			}
//		}
//
//		template<typename Type, typename Allocator>
//		void vector_base<Type, Allocator>::swap(this_type& other)
//		{
//			using simstd::swap;
//			swap(begin, other.begin);
//			swap(end, other.end);
//			swap(end_of_storage, other.end_of_storage);
//		}
//
//		template<typename Type, typename Allocator>
//		void vector_base<Type, Allocator>::create_storage(size_type capa)
//		{
//			end = begin = (capa) ? traits_type::allocate(allocator, capa) : 0;
//			end_of_storage = begin + (begin ? capa : 0);
//		}
//
//		template<typename Type, typename Allocator>
//		template<typename ... Args>
//		void vector_base<Type, Allocator>::construct(pointer ptr, Args&&... args)
//		{
//			traits_type::construct(allocator, ptr, simstd::forward<Args>(args)...);
//		}
//
//		template<typename Type, typename Allocator>
//		void vector_base<Type, Allocator>::destroy(pointer first, pointer last)
//		{
//			for (; first != last; ++first)
//				traits_type::destroy(allocator, first);
//		}
//
//		template<typename Type, typename Allocator>
//		void vector_base<Type, Allocator>::reserve(size_type newCapacity)
//		{
//			if (capacity() < newCapacity) {
//				this_type tmp(newCapacity, allocator);
//				tmp.end = simstd::uninitialized_copy(simstd::make_move_iterator(begin), simstd::make_move_iterator(end), tmp.end);
//				tmp.swap(*this);
//			}
//		}
//
//		template<typename Type, typename Allocator>
//		void vector_base<Type, Allocator>::clear()
//		{
//			destroy(begin, end);
//			end = begin;
//		}
//
//		template<typename Type, typename Allocator>
//		typename vector_base<Type, Allocator>::size_type vector_base<Type, Allocator>::size() const
//		{
//			return end - begin;
//		}
//
//		template<typename Type, typename Allocator>
//		typename vector_base<Type, Allocator>::size_type vector_base<Type, Allocator>::max_size() const
//		{
//			return 0xFFFFFFFFu;
//		}
//
//		template<typename Type, typename Allocator>
//		typename vector_base<Type, Allocator>::size_type vector_base<Type, Allocator>::capacity() const
//		{
//			return end_of_storage - begin;
//		}
//
//		template<typename Type, typename Allocator>
//		bool vector_base<Type, Allocator>::check_capacity_if_size_grows(size_type addToSize) const
//		{
//			if (addToSize > static_cast<size_type>(end_of_storage - end))
//				return false;
//			return true;
//		}
//
//		template<typename Type, typename Allocator>
//		typename vector_base<Type, Allocator>::size_type vector_base<Type, Allocator>::get_new_capacity(size_type addToSize) const
//		{
//			return size() + simstd::max(size_type(4), simstd::max(size(), addToSize));
//		}
//
//		template<typename Type, typename Allocator>
//		void vector_base<Type, Allocator>::adjust_capacity(size_type addToSize)
//		{
//			if (!check_capacity_if_size_grows(addToSize))
//				reserve(get_new_capacity(addToSize));
//		}

}

#endif
