#ifndef BASIS_MEMORY_ALLOCATOR_HPP_
#define BASIS_MEMORY_ALLOCATOR_HPP_

namespace simstd {

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
		void deallocate(pointer ptr, const char* function, int line, size_type cnt);

		size_type max_size() const noexcept;

		template<typename PtrType, typename ... Args>
		void construct(PtrType* ptr, Args&&... args);

		template<typename PtrType>
		void destroy(PtrType* ptr);
	};

	template<typename Type>
	typename allocator<Type>::pointer allocator<Type>::address(reference r) const noexcept
	{
		return simstd::addressof(r);
	}

	template<typename Type>
	typename allocator<Type>::const_pointer allocator<Type>::address(const_reference r) const noexcept
	{
		return simstd::addressof(r);
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
	void allocator<Type>::deallocate(pointer ptr, const char* function, int line, size_type /*cnt*/)
	{
		HostDelete2(ptr, function, line);
	}

	template<typename Type>
	typename allocator<Type>::size_type allocator<Type>::max_size() const noexcept
	{
		return static_cast<size_type>(-1) / sizeof(Type);
	}

	template<typename Type>
	template<typename PtrType, typename ... Args>
	void allocator<Type>::construct(PtrType* ptr, Args&&... args)
	{
		::new (static_cast<void*>(ptr), simstd::nothrow) Type(simstd::forward<Args>(args)...);
	}

	template<typename Type>
	template<typename PtrType>
	void allocator<Type>::destroy(PtrType* ptr)
	{
		UNUSED(ptr); // stupid VC
		ptr->~Type();
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

}

#endif
