#ifndef LIBBASE_PVT_MEMORY_HPP_
#define LIBBASE_PVT_MEMORY_HPP_

#include <windows.h>

#include <type_traits>

namespace Memory {
#ifdef MEMORY_DEBUG
	namespace Watchdog
	{
		typedef void (*pfunc)();
		extern size_t allocations;
		extern size_t deletions;
		extern uint64_t allocations_size;
		extern uint64_t deletions_size;

		extern pfunc on_delete;
	};

#endif

	typedef HANDLE Heap_t;

	inline Memory::Heap_t get_heap()
	{
		return ::GetProcessHeap();
	}

	template<typename Pointer>
	inline size_t size(Pointer in)
	{
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
		return (in) ? ::HeapSize(get_heap(), 0, (void*)in) : 0;
	}

	template<typename Pointer>
	inline Pointer malloc(size_t size, DWORD flags = 0/*HEAP_ZERO_MEMORY*/)
	{
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
#ifdef MEMORY_DEBUG
		Watchdog::allocations++;
		Watchdog::allocations_size += size;
#endif
		return static_cast<Pointer>(::HeapAlloc(get_heap(), flags, size));
	}

	template<typename Pointer>
	inline Pointer calloc(size_t count, DWORD flags = 0)
	{
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
		Pointer tmp_ptr = nullptr;
#ifdef MEMORY_DEBUG
		Watchdog::allocations++;
		Watchdog::allocations_size += sizeof(*tmp_ptr) * count;
#endif
		return static_cast<Pointer>(::HeapAlloc(get_heap(), flags | HEAP_ZERO_MEMORY, sizeof(*tmp_ptr) * count));
	}

	template<typename Pointer>
	inline Pointer ealloc(DWORD flags = 0)
	{
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
		Pointer tmp_ptr = nullptr;
#ifdef MEMORY_DEBUG
		Watchdog::allocations++;
		Watchdog::allocations_size += sizeof(*tmp_ptr);
#endif
		return static_cast<Pointer>(::HeapAlloc(get_heap(), flags | HEAP_ZERO_MEMORY, sizeof(*tmp_ptr)));
	}

	template<typename Pointer>
	inline void free(Pointer & in)
	{
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
#ifdef MEMORY_DEBUG
		Watchdog::deletions++;
		Watchdog::deletions_size += Memory::size(in);
		if (Watchdog::deletions_size == Watchdog::allocations_size && Watchdog::deletions == Watchdog::allocations)
			printf("There is no leaks\n");
#endif
		::HeapFree(get_heap(), 0, *(void**)(&in));
		*(void**)(&in) = nullptr;
	}

	template<typename Pointer>
	inline bool realloc(Pointer & in, size_t size, DWORD flags = HEAP_ZERO_MEMORY)
	{
#ifdef MEMORY_DEBUG
		if (in) {
			Watchdog::deletions++;
			Watchdog::deletions_size += Memory::size(in);
		}
		Watchdog::allocations++;
		Watchdog::allocations_size += size;
#endif
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
		return (in = static_cast<Pointer>((in) ? ::HeapReAlloc(get_heap(), flags, (void*)in, size) : ::HeapAlloc(get_heap(), flags, size)));
	}

	template<typename Pointer1, typename Pointer2>
	inline bool compare(Pointer1 m1, Pointer2 m2, size_t size)
	{
		static_assert(std::is_pointer<Pointer1>::value, "Pointer type is required");
		static_assert(std::is_pointer<Pointer2>::value, "Pointer type is required");
		return ::memcmp((const void*)m1, (const void*)m2, size) == 0;
	}

	template<typename Pointer1, typename Pointer2>
	inline Pointer1 copy(Pointer1 dest, Pointer2 src, size_t size)
	{
		static_assert(std::is_pointer<Pointer1>::value, "Pointer type is required");
		static_assert(std::is_pointer<Pointer2>::value, "Pointer type is required");
		//return ::memcpy_s(dest, sour, size);
		return static_cast<Pointer1>(::memcpy((void*)dest, (void*)src, size));
	}

	template<typename Pointer1, typename Pointer2>
	inline Pointer1 move(Pointer1 dest, Pointer2 src, size_t size)
	{
		static_assert(std::is_pointer<Pointer1>::value, "Pointer type is required");
		static_assert(std::is_pointer<Pointer2>::value, "Pointer type is required");
		return static_cast<Pointer1>(::memmove((void*)dest, (void*)src, size));
	}

	template<typename Pointer1, typename Pointer2>
	inline Pointer1 dup(Pointer2 src, size_t size)
	{
		static_assert(std::is_pointer<Pointer1>::value, "Pointer type is required");
		static_assert(std::is_pointer<Pointer2>::value, "Pointer type is required");
		return Memory::copy(Memory::malloc<Pointer1>(size), src, size);
	}

	template<typename Pointer>
	inline Pointer fill(Pointer in, size_t bytes, int fill)
	{
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
		return static_cast<Pointer>(::memset((void*)in, fill, bytes));
	}

	template<typename NonPointer>
	inline void fill(NonPointer in, int fill)
	{
		static_assert(!std::is_pointer<NonPointer>::value, "Nonpointer type is required");
		::memset((void*)in, fill, sizeof(in));
	}

	template<typename Pointer>
	inline Pointer zero(Pointer in, size_t bytes)
	{
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
		return static_cast<Pointer>(::memset((void*)in, 0, bytes));
	}

	template<typename NonPointer>
	inline void zero(NonPointer & in)
	{
		static_assert(!std::is_pointer<NonPointer>::value, "Nonpointer type is required");
		::memset((void*)&in, 0, sizeof(in));
	}
}

//#if defined(NoStdNew) || defined(NO_STD_NEW)
//inline void * operator new(size_t size) noexcept
//{
//	return Memory::malloc<void*>(size, HEAP_ZERO_MEMORY);
//}
//
//inline void * operator new [](size_t size) noexcept
//{
//	return ::operator new(size);
//}
//
//inline void operator delete(void * in) noexcept
//{
//	Memory::free(in);
//}
//
//inline void operator delete [](void * ptr) noexcept
//{
//	::operator delete(ptr);
//}
//#endif

#endif