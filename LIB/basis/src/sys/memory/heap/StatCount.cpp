#include <basis/configure.hpp>
#include <basis/sys/~memory/heap/Stat.hpp>
#include <basis/sys/traceback.hpp>

#include <basis/simstd/vector>

memory::heap::AllocatedItem::AllocatedItem(void* ptr, uint64_t size, const char* function, int line) :
	size(size),
	ptr(ptr),
	function(function),
	line(line)
{
	TraceFuncLn();
}

memory::heap::StatCount::~StatCount()
{
	TraceFuncLn();
}

memory::heap::StatCount::StatCount():
	allocations(),
	frees(),
	allocSize(),
	freeSize()
{
	TraceFuncLn();
}

void* memory::heap::StatCount::commit_alloc(void* ptr, uint64_t size, const char* function, int line)
{
//	TraceFuncLn();
	UNUSED(function);
	UNUSED(line);

	if (ptr) {
		++allocations;
		allocSize += size;
	}

	return ptr;
}

void memory::heap::StatCount::commit_free(const void* ptr, uint64_t size, const char* function, int line)
{
//	TraceFuncLn();
	UNUSED(function);
	UNUSED(line);

	if (ptr) {
		++frees;
		freeSize += size;
	}
}

void memory::heap::StatCount::get_database(AllocatedItem*& db, size_t& size) const
{
	db = nullptr;
	size = 0;
}

void memory::heap::StatCount::free_database(AllocatedItem* db) const
{
	delete db;
}
