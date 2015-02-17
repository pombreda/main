#ifndef BASIS_MEMORY_ALLOCATORHEAP_HPP_
#define BASIS_MEMORY_ALLOCATORHEAP_HPP_

namespace simstd {

	template<typename Type, typename HeapType>
	class AllocatorHeap {
	public:
		using value_type      = Type;
		using pointer         = Type*;
		using const_pointer   = const Type*;
		using reference       = Type&;
		using const_reference = const Type&;
		using size_type       = size_t;
		using difference_type = ptrdiff_t;

		template<typename OType>
		struct rebind {
			using other = AllocatorHeap<OType, HeapType>;
		};

		~AllocatorHeap() = default;

		template<class OType>
		AllocatorHeap(const AllocatorHeap<OType, HeapType>& /*other*/) noexcept {}
		AllocatorHeap() noexcept = default;
		AllocatorHeap(const AllocatorHeap& other) noexcept = default;

		pointer       address(reference r) const noexcept;
		const_pointer address(const_reference r) const noexcept;

		pointer allocate(size_type cnt, simstd::allocator<void>::const_pointer hint = 0);
		void    deallocate(pointer ptr, size_type cnt);
		pointer allocate(size_type cnt, const char* function, int line, allocator<void>::const_pointer hint = nullptr);
		void    deallocate(pointer ptr, const char* function, int line, size_type cnt);

		size_type max_size() const noexcept;

		template<typename PtrType, typename ... Args>
		void construct(PtrType* ptr, Args&&... args);

		template<typename PtrType>
		void destroy(PtrType* ptr);
	};

	template<typename Type, typename HeapType>
	typename AllocatorHeap<Type, HeapType>::pointer AllocatorHeap<Type, HeapType>::address(reference r) const noexcept
	{
		return simstd::addressof(r);
	}

	template<typename Type, typename HeapType>
	typename AllocatorHeap<Type, HeapType>::const_pointer AllocatorHeap<Type, HeapType>::address(const_reference r) const noexcept
	{
		return simstd::addressof(r);
	}

	template<typename Type, typename HeapType>
	typename AllocatorHeap<Type, HeapType>::pointer AllocatorHeap<Type, HeapType>::allocate(size_type cnt, simstd::allocator<void>::const_pointer /*hint*/)
	{
		return static_cast<pointer>(HostAlloc(HeapType, sizeof(Type) * cnt));
	}

	template<typename Type, typename HeapType>
	void AllocatorHeap<Type, HeapType>::deallocate(pointer ptr, size_type /*cnt*/)
	{
		HostFree(HeapType, ptr);
	}

	template<typename Type, typename HeapType>
	typename AllocatorHeap<Type, HeapType>::pointer AllocatorHeap<Type, HeapType>::allocate(size_type cnt, const char* function, int line, allocator<void>::const_pointer /*hint*/)
	{
		return static_cast<pointer>(HostAlloc2(HeapType, sizeof(Type) * cnt, function, line));
	}

	template<typename Type, typename HeapType>
	void AllocatorHeap<Type, HeapType>::deallocate(pointer ptr, const char* function, int line, size_type /*cnt*/)
	{
		HostFree2(HeapType, ptr, function, line);
	}

	template<typename Type, typename HeapType>
	typename AllocatorHeap<Type, HeapType>::size_type AllocatorHeap<Type, HeapType>::max_size() const noexcept
	{
		return HeapType::size() / sizeof(Type);
	}

	template<typename Type, typename HeapType>
	template<typename PtrType, typename ... Args>
	void AllocatorHeap<Type, HeapType>::construct(PtrType* ptr, Args&&... args)
	{
		::new (static_cast<void*>(ptr), simstd::nothrow) Type(simstd::forward<Args>(args)...);
	}

	template<typename Type, typename HeapType>
	template<typename PtrType>
	void AllocatorHeap<Type, HeapType>::destroy(PtrType* ptr)
	{
		ptr->~Type();
	}

	template<typename Type, typename HeapType>
	bool operator ==(const AllocatorHeap<Type, HeapType>&, const AllocatorHeap<Type, HeapType>&) noexcept
	{
		return true;
	}

	template<typename Type1, typename Type2, typename HeapType>
	bool operator ==(const AllocatorHeap<Type1, HeapType>&, const AllocatorHeap<Type2, HeapType>&) noexcept
	{
		return true;
	}

	template<typename Type, typename HeapType>
	bool operator !=(const AllocatorHeap<Type, HeapType>&, const AllocatorHeap<Type, HeapType>&) noexcept
	{
		return false;
	}

	template<typename Type1, typename Type2, typename HeapType>
	bool operator !=(const AllocatorHeap<Type1, HeapType>&, const AllocatorHeap<Type2, HeapType>&) noexcept
	{
		return false;
	}

}

#endif
