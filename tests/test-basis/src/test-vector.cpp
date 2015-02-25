#include <tests.hpp>

#include <basis/sys/console.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/memory.hpp>
#include <basis/simstd/memory>
#include <basis/simstd/vector>

#include <basis/tst/A.hpp>

namespace {
	typedef memory::heap::DecoratorStat<memory::heap::Default, memory::heap::StatCount, tst::A> heap_type;
	typedef simstd::AllocatorHeap<tst::A, heap_type> Allocator;
	typedef simstd::vector<tst::A, Allocator> vector_type;
}

void test_vector()
{
	LogWarn(L"\n");

	{
//		vector_type* vec1 = static_cast<vector_type*>(HostAlloc(heap_type, sizeof(vector_type)));
//		new (vec1, simstd::nothrow) vector_type();
		vector_type* vec1 = new vector_type;
		LogReport(L"sizeof: %Iu\n", sizeof(*vec1));

		LogReport(L"size: %Iu, capa: %Iu\n", vec1->size(), vec1->capacity());
		vec1->emplace_back(0);
		vec1->emplace_back(1);
		vec1->emplace_back(2);
		vec1->emplace_back(3);
		vec1->emplace_back(4);
		vec1->emplace_back(5);
		vec1->emplace_back(6);
		LogReport(L"size: %Iu, capa: %Iu\n", vec1->size(), vec1->capacity());
		vec1->emplace_back(7);
		vec1->emplace_back(8);
		vec1->emplace_back(9);
		vec1->emplace_back(10);
		LogReport(L"size: %Iu, capa: %Iu\n", vec1->size(), vec1->capacity());

		delete vec1;
//		vec1->~vector();
//		HostFree(heap_type, vec1);
	}

	{
		const auto stat = heap_type::get_stat();
		LogReport(L"stat alloc: %I64u, %I64u\n", stat.get_allocations(), stat.get_allocations_size());
		LogReport(L"stat free : %I64u, %I64u\n", stat.get_frees(), stat.get_frees_size());
		LogReport(L"stat diff : %I64d\n", stat.get_allocations_size() - stat.get_frees_size());
	}

	tst::_vector(console::printf);
}
