#ifndef BASIS_MEMORY_ALLOCATOR_HPP_
#define BASIS_MEMORY_ALLOCATOR_HPP_

namespace simstd
{
	template<>
	class allocator<void>
	{
	public:
		using value_type = void;
		using pointer = void*;
		using const_pointer = const void*;
		using size_type = size_t;
		using difference_type = ptrdiff_t;

		template<typename OType>
		struct rebind
		{
			using other = allocator<OType>;
		};
	};

	template<typename Type>
	class allocator
	{
	public:
		using value_type = Type;
		using pointer = Type*;
		using const_pointer = const Type*;
		using reference = Type&;
		using const_reference = const Type&;
		using size_type = size_t;
		using difference_type = ptrdiff_t;

		template<typename OType>
		struct rebind
		{
			using other = allocator<OType>;
		};

		~allocator() = default;

		template<class OType>
		allocator(const allocator<OType>& /*other*/) noexcept {}
		allocator() noexcept = default;
		allocator(const allocator& other) noexcept = default;

		pointer address(reference r) const noexcept;
		const_pointer address(const_reference r) const noexcept;

		pointer allocate(size_type cnt, allocator<void>::const_pointer hint = nullptr);
		void deallocate(pointer ptr, size_type cnt);
		pointer allocate(size_type cnt, const char* function, int line, allocator<void>::const_pointer hint = nullptr);
		void deallocate(pointer ptr, size_type cnt, const char* function, int line);

		size_type max_size() const noexcept;

		template<typename PtrType, typename ... Args>
		void construct(PtrType* ptr, Args&&... args);

		template<typename PtrType>
		void destroy(PtrType* ptr);
	};

	template<typename Type>
	typename allocator<Type>::pointer allocator<Type>::address(reference r) const noexcept
	{
		return addressof(r);
	}

	template<typename Type>
	typename allocator<Type>::const_pointer allocator<Type>::address(const_reference r) const noexcept
	{
		return addressof(r);
	}

	template<typename Type>
	typename allocator<Type>::pointer allocator<Type>::allocate(size_type cnt, allocator<void>::const_pointer /*hint*/)
	{
		return static_cast<pointer>(HostNew(sizeof(Type) * cnt));
	}

	template<typename Type>
	void allocator<Type>::deallocate(pointer ptr, size_type /*cnt*/)
	{
		HostDelete(ptr);
	}

	template<typename Type>
	typename allocator<Type>::pointer allocator<Type>::allocate(size_type cnt, const char* function, int line, allocator<void>::const_pointer /*hint*/)
	{
		return static_cast<pointer>(HostNew2(sizeof(Type) * cnt, function, line));
	}

	template<typename Type>
	void allocator<Type>::deallocate(pointer ptr, size_type /*cnt*/, const char* function, int line)
	{
		HostDelete2(ptr, function, line);
	}

	template<typename Type>
	typename allocator<Type>::size_type allocator<Type>::max_size() const noexcept
	{
		return static_cast<size_type>(-1) / sizeof(Type);
	}

	template<typename Type>
	template<typename PtrType, typename... Args>
	void allocator<Type>::construct(PtrType* ptr, Args&&... args)
	{
		::new (static_cast<void*>(ptr), nothrow) PtrType(forward<Args>(args)...);
	}

	template<typename Type>
	template<typename PtrType>
	void allocator<Type>::destroy(PtrType* ptr)
	{
		UNUSED(ptr); // stupid VC
		ptr->~PtrType();
	}

	template<typename Type1, typename Type2>
	bool operator ==(const allocator<Type1>&, const allocator<Type2>&) noexcept
	{
		return true;
	}

	template<typename Type>
	bool operator ==(const allocator<Type>&, const allocator<Type>&) noexcept
	{
		return true;
	}

	template<typename Type1, typename Type2>
	bool operator !=(const allocator<Type1>&, const allocator<Type2>&) noexcept
	{
		return false;
	}

	template<typename Type>
	bool operator !=(const allocator<Type>&, const allocator<Type>&) noexcept
	{
		return false;
	}

	namespace pvt
	{
		template<typename Allocator, typename Type>
		class allocator_rebind_helper
		{
			template<typename OAllocator, typename OType>
			static constexpr true_type chk_allocator(typename OAllocator::template rebind<OType>::other*);

			template<typename, typename>
			static constexpr false_type chk_allocator(...);

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

		template<template<typename, typename...> class Allocator, typename Type, typename OType, typename... Args>
		struct allocator_rebind<Allocator<OType, Args...>, Type, false>
		{
			using type = Allocator<Type, Args...>;
		};
	}

	template<typename Allocator>
	struct allocator_traits
	{
		using allocator_type = Allocator;
		using value_type = typename Allocator::value_type;
		using size_type = typename Allocator::size_type;
		using difference_type = typename Allocator::difference_type;
		using pointer = typename Allocator::pointer;
		using const_pointer = typename Allocator::const_pointer;
		using void_pointer = void*;
		using const_void_pointer = const void*;

		template<typename Type>
		using rebind_alloc = typename pvt::allocator_rebind<Allocator, Type>::type;

		template<typename Type>
		using rebind_traits = allocator_traits<rebind_alloc<Type>>;

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
	typename allocator_traits<Allocator>::pointer allocator_traits<Allocator>::allocate(Allocator& a, size_type n, const_void_pointer hint)
	{
		return a.allocate(n, hint);
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
		TraceFuncLn();
		a.construct(ptr, forward<Args>(args)...);
	}

	template<typename Allocator>
	template<class T>
	void allocator_traits<Allocator>::destroy(Allocator& a, T* ptr)
	{
		TraceFuncLn();
		a.destroy(ptr);
	}

	template<typename Allocator>
	typename allocator_traits<Allocator>::size_type allocator_traits<Allocator>::max_size(const Allocator& a)
	{
		return a.max_size();
	}
}

#endif
