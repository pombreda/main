#include <basis/configure.hpp>
#include <basis/sys/memory.hpp>

namespace memory
{
	namespace heap
	{
		void Default::init(size_t)
		{
			TraceFuncLn();
		}

		void Default::destroy()
		{
			TraceFuncLn();
		}

		size_t Default::size()
		{
			return static_cast<size_t>(-1);
		}

		void Default::lock()
		{
		}

		void Default::unlock()
		{
		}

		void* Default::alloc(size_t size, const char*, int)
		{
//			TraceFuncLn();
			return ::HeapAlloc(::GetProcessHeap(), 0, size);
		}

		void* Default::realloc(void* ptr, size_t size, const char*, int)
		{
//			TraceFuncLn();
			return ptr ? ::HeapReAlloc(::GetProcessHeap(), 0, ptr, size) : ::HeapAlloc(::GetProcessHeap(), 0, size);
		}

		void Default::free(const void* ptr, const char*, int)
		{
//			TraceFuncLn();
			::HeapFree(::GetProcessHeap(), 0, const_cast<void*>(ptr));
		}

		size_t Default::size(const void* ptr)
		{
			return ptr ? ::HeapSize(::GetProcessHeap(), 0, ptr) : 0;
		}

		const char* Default::get_name()
		{
			return "Default";
		}
	}
}
