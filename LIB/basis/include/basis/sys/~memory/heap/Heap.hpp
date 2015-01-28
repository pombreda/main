#ifndef BASIS_SYS_MEMORY_HEAP_HEAP_HPP_
#define BASIS_SYS_MEMORY_HEAP_HEAP_HPP_

#include <basis/configure.hpp>
#include <basis/sys/~memory/heap/Default.hpp>
#include <basis/sys/~memory/heap/Special.hpp>
#include <basis/sys/~memory/heap/Stat.hpp>

#if defined(MEMORY_TRACE) && MEMORY_TRACE == 2
#   define HostAlloc(HostType, Size)        HostType::alloc(Size, __PRETTY_FUNCTION__, __LINE__)
#   define HostRealloc(HostType, Ptr, Size) HostType::realloc(Ptr, Size, __PRETTY_FUNCTION__, __LINE__)
#   define HostFree(HostType, Ptr)          HostType::free(Ptr, __PRETTY_FUNCTION__, __LINE__)
#   define HostNew(Size)                    ::operator new(Size, simstd::nothrow, __PRETTY_FUNCTION__, __LINE__)
#   define HostDelete(Ptr)                  ::operator delete(Ptr, simstd::nothrow, __PRETTY_FUNCTION__, __LINE__);
#elif defined(MEMORY_TRACE) && MEMORY_TRACE == 1
#   define HostAlloc(HostType, Size) HostType::alloc(Size, __FUNCTION__, __LINE__)
#   define HostRealloc(HostType, Ptr, Size) HostType::realloc(Ptr, Size, __FUNCTION__, __LINE__)
#   define HostFree(HostType, Ptr) HostType::free(Ptr, __FUNCTION__, __LINE__)
#   define HostNew(Size)                    ::operator new(Size, simstd::nothrow, __FUNCTION__, __LINE__)
#   define HostDelete(Ptr)                  ::operator delete(Ptr, simstd::nothrow, __FUNCTION__, __LINE__);
#else
#   define HostAlloc(HostType, Size) HostType::alloc(Size, "", 0)
#   define HostRealloc(HostType, Ptr, Size) HostType::realloc(Ptr, Size, "", 0)
#   define HostFree(HostType, Ptr) HostType::free(Ptr, "", 0)
#   define HostNew(Size)                    ::operator new(Size, simstd::nothrow, "", 0)
#   define HostDelete(Ptr)                  ::operator delete(Ptr, simstd::nothrow, "", 0);
#endif

#define HostAlloc2(HostType, Size, Func, Line) HostType::alloc(Size, Func, Line)
#define HostRealloc2(HostType, Ptr, Size, Func, Line) HostType::realloc(Ptr, Size, Func, Line)
#define HostFree2(HostType, Ptr, Func, Line) HostType::free(Ptr, Func, Line)

namespace memory {
	namespace heap {

		typedef DecoratorStat<Default, StatCount> DefaultStatCount;

		typedef DecoratorStat<Default, StatLog> DefaultStatLogged;

#if !defined(MEMORY_LOGGING) || MEMORY_LOGGING == 0
		typedef DefaultStatCount DefaultStat;
#else
		typedef DefaultStatLogged DefaultStat;
#endif

	}
}

namespace memory {
	namespace heap {

		struct Heap {
			~Heap();
			Heap(size_t size = 0);
			size_t size() const;

			void*  alloc(size_t size);
			void   free(const void* ptr);
			size_t size(const void* ptr) const;

		private:
			HANDLE m_heap;
		};

	}
}

#endif
