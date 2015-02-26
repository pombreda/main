#ifndef BASIS_MEMORY_ALLOCATOR_TRAITS_HPP_
#define BASIS_MEMORY_ALLOCATOR_TRAITS_HPP_

namespace simstd {

	namespace pvt {
		template<typename Allocator, typename Type>
		class allocator_rebind_helper
		{
			template<typename OAllocator, typename OType>
			static constexpr simstd::true_type chk_allocator(typename OAllocator::template rebind<OType>::other*);

			template<typename, typename>
			static constexpr simstd::false_type chk_allocator(...);

		public:
			using type = decltype(chk_allocator<Allocator, Type>(nullptr));
		};

		template<typename Allocator, typename Type, bool = allocator_rebind_helper<Allocator, Type>::type::value>
		struct allocator_rebind;

		template<typename Allocator, typename Type>
		struct allocator_rebind<Allocator, Type, true>
		{
			using type = typename Allocator::template rebind<Type>::other;
		};

		template<template<typename, typename...> class Allocator, typename Type, typename _Up, typename... _Args>
		struct allocator_rebind<Allocator<_Up, _Args...>, Type, false>
		{
			using type = Allocator<Type, _Args...>;
		};
	}

	template<typename Allocator>
	struct allocator_traits
	{
		typedef Allocator                           allocator_type;
		typedef typename Allocator::value_type      value_type;
		typedef typename Allocator::size_type       size_type;
		typedef typename Allocator::difference_type difference_type;
//		typedef typename Allocator::reference       reference;
//		typedef typename Allocator::const_reference const_reference;
		typedef typename Allocator::pointer         pointer;
		typedef typename Allocator::const_pointer   const_pointer;
		typedef void*                               void_pointer;
		typedef const void*                         const_void_pointer;

		template<typename Type>
		using rebind_alloc = typename pvt::allocator_rebind<Allocator, Type>::type;

		template<typename Type>
		using rebind_traits = simstd::allocator_traits<rebind_alloc<Type>>;

		static pointer allocate(Allocator& a, size_type n);

		static pointer allocate(Allocator& a, size_type n, const_void_pointer hint);

		static void deallocate(Allocator& a, pointer ptr, size_type n);

		template<class T, class ... Args>
		static void construct(Allocator& a, T* ptr, Args&&... args);

		template<class T>
		static void destroy(Allocator& a, T* p);

		static size_type max_size(const Allocator& a);
	};

	template<typename Allocator>
	typename allocator_traits<Allocator>::pointer allocator_traits<Allocator>::allocate(Allocator& a, size_type n)
	{
		return a.allocate(n);
	}

	template<typename Allocator>
	typename allocator_traits<Allocator>::pointer allocator_traits<Allocator>::allocate(Allocator& a, size_type n, const_void_pointer /*hint*/)
	{
		return a.allocate(n); // TODO hint
	}

	template<typename Allocator>
	void allocator_traits<Allocator>::deallocate(Allocator& a, pointer ptr, size_type n)
	{
		a.deallocate(ptr, n);
	}

	template<typename Allocator>
	template<class T, class... Args>
	void allocator_traits<Allocator>::construct(Allocator& a, T* ptr, Args&&... args)
	{
		TraceFunc();
		a.construct(ptr, simstd::forward<Args>(args)...);
	}

	template<typename Allocator>
	template<class T>
	void allocator_traits<Allocator>::destroy(Allocator& a, T* ptr)
	{
		a.destroy(ptr); // TODO
	}

	template<typename Allocator>
	typename allocator_traits<Allocator>::size_type allocator_traits<Allocator>::max_size(const Allocator& a)
	{
		return a.max_size(); //TODO
	}

}

#endif
