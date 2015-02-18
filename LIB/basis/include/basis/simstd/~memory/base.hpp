﻿#ifndef BASIS_MEMORY_BASE_HPP_
#define BASIS_MEMORY_BASE_HPP_

namespace simstd {

	namespace pvt {
//		template<typename Type>
//		Type* _addressof(Type& ref)
//		{
//			return reinterpret_cast<Type*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(ref)));
//		}

		template<typename Type>
		Type* _allocate(size_t cnt)
		{
			return static_cast<Type*>(HostNew(sizeof(Type) * cnt));
		}

		template<typename Type>
		void _deallocate(Type* ptr, size_t /*cnt*/ = 0)
		{
			HostDelete(ptr);
		}

//		template<typename Type1, typename Type2>
//		void _construct(Type1* ptr, const Type2& val)
//		{
//			::new (static_cast<void*>(ptr), simstd::nothrow) Type1(val);
//		}

		template<typename Allocator, typename ForwardIterator>
		ForwardIterator _construct_default(Allocator& allocator, ForwardIterator first, ForwardIterator last)
		{
			using traits_type = allocator_traits<Allocator>;
			for (; first != last; ++first)
				traits_type::construct(allocator, simstd::addressof(*first));
			return first;
		}

		template<typename Allocator, typename ForwardIterator, typename Type>
		ForwardIterator _construct_copy(Allocator& allocator, ForwardIterator first, ForwardIterator last, const Type& value)
		{
			typedef allocator_traits<Allocator> traits_type;
			for (; first != last; ++first)
				traits_type::construct(allocator, simstd::addressof(*first), value);
			return first;
		}

		template<typename Type, typename... Args>
		void _construct(Type* ptr, Args&&... args)
		{
			::new (static_cast<void*>(ptr), simstd::nothrow) Type(simstd::forward<Args>(args)...);
		}

		template<typename Type>
		void _destroy(Type* ptr)
		{
			ptr->~Type();
		}

		template<typename Type, typename ForwardIterator>
		void _destroy(ForwardIterator first, ForwardIterator last)
		{
			for (; first != last; ++first)
				first->~Type();
		}

		template<typename Allocator, typename ForwardIterator>
		void _destroy(Allocator& allocator, ForwardIterator first, ForwardIterator last)
		{
			using traits_type = allocator_traits<Allocator>;
			for (; first != last; ++first)
				traits_type::destroy(allocator, simstd::addressof(*first));
		}

	}

//	template<bool>
//	struct _Destroy_aux {
//		template<typename ForwardIterator>
//		static void _destroy(ForwardIterator first, ForwardIterator last)
//		{
//			for (; first != last; ++first)
//				simstd::_destroy(simstd::_addressof(*first));
//		}
//	};
//
//	template<>
//	struct _Destroy_aux<true> {
//		template<typename ForwardIterator>
//		static void _destroy(ForwardIterator, ForwardIterator)
//		{
//		}
//	};
//
//	template<typename ForwardIterator>
//	void _destroy(ForwardIterator first, ForwardIterator last)
//	{
//		typedef typename iterator_traits<ForwardIterator>::value_type Value_type;
//		simstd::_Destroy_aux<__has_trivial_destructor(Value_type)>::_destroy(first, last);
//	}
//
//	template<typename ForwardIterator, typename Allocator>
//	void _destroy(ForwardIterator first, ForwardIterator last, Allocator& alloc)
//	{
//		for (; first != last; ++first)
//			alloc.destroy(simstd::_addressof(*first));
//	}
//
//	template<typename ForwardIterator, typename Type>
//	void _destroy(ForwardIterator first, ForwardIterator last, allocator<Type>&)
//	{
//		_destroy(first, last);
//	}

	template<typename Type>
	simstd::pair<Type*, ptrdiff_t> get_temporary_buffer(ptrdiff_t cnt)
	{
		Type* ptr = static_cast<Type*>(HostNew(sizeof(Type) * cnt));
		return simstd::pair<Type*, ptrdiff_t>(ptr, ptr ? cnt : 0);
	}

	template<typename Type>
	void return_temporary_buffer(Type* ptr)
	{
		HostDelete(ptr);
	}

	template<typename InputIt, typename ForwardIt>
	ForwardIt uninitialized_copy(InputIt first, InputIt last, ForwardIt d_first)
	{
		for (; first != last; ++first, ++d_first) {
			simstd::pvt::_construct(simstd::addressof(*d_first), *first);
		}
		return d_first;
	}

	template<typename InputIt, typename ForwardIt, typename Allocator>
	ForwardIt uninitialized_copy_a(InputIt first, InputIt last, ForwardIt d_first, Allocator& allocator)
	{
		using alloc_traits = simstd::allocator_traits<Allocator>;
		for (; first != last; ++first, ++d_first) {
			alloc_traits::construct(allocator, simstd::addressof(*d_first), *first);
		}
		return d_first;
	}

//	template<typename InputIt, typename ForwardIt, typename Allocator>
//	ForwardIt uninitialized_move_a(InputIt first, InputIt last, ForwardIt d_first, Allocator& allocator)
//	{
//		return simstd::uninitialized_copy_a(simstd::make_move_iterator(first), simstd::make_move_iterator(last), d_first, allocator);
//	}

	template<typename InputIt, typename Size, typename ForwardIt>
	ForwardIt uninitialized_copy_n(InputIt first, Size cnt, ForwardIt d_first)
	{
		for (; cnt > 0; ++first, ++d_first, --cnt) {
			simstd::pvt::_construct(simstd::addressof(*d_first), *first);
		}
		return d_first;
	}

	template<typename ForwardIt, typename Type>
	ForwardIt uninitialized_fill(ForwardIt first, ForwardIt last, const Type& val)
	{
		for (; first != last; ++first) {
			simstd::pvt::_construct(simstd::addressof(*first), val);
		}
		return first;
	}

	template<typename ForwardIt, typename Size, typename Type>
	ForwardIt uninitialized_fill_n(ForwardIt first, Size cnt, const Type& val)
	{
		for (; cnt > 0; ++first, --cnt) {
			simstd::pvt::_construct(simstd::addressof(*first), val);
		}
		return first;
	}

	template<typename ForwardIt, typename Size, typename Type, typename Allocator>
	ForwardIt uninitialized_fill_n_a(ForwardIt first, Size cnt, const Type& val, Allocator& allocator)
	{
		using alloc_traits = simstd::allocator_traits<Allocator>;
		for (; cnt > 0; ++first, --cnt) {
			alloc_traits::construct(allocator, simstd::addressof(*first), val);
		}
		return first;
	}

	template<typename ForwardIt, typename Size, typename Allocator>
	ForwardIt uninitialized_default_n_a(ForwardIt first, Size cnt, Allocator& allocator)
	{
		using alloc_traits = simstd::allocator_traits<Allocator>;
		for (; cnt > 0; ++first, --cnt) {
			alloc_traits::construct(allocator, simstd::addressof(*first));
		}
		return first;
	}

}

#endif
