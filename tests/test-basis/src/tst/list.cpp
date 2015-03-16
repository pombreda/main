#include <basis/tst.hpp>
#include <basis/sys/console.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/memory.hpp>

#include <basis/simstd/algorithm>
#include <basis/simstd/chrono>
#include <basis/simstd/memory>
#include <basis/simstd/list>
#include <list>
#include <ctime>

struct HeapTag {};
using heap_type = memory::heap::DecoratorStat<memory::heap::Special<HeapTag>, memory::heap::StatCount, HeapTag>;
//using EqAlloc = simstd::allocator<A>;
using EqAlloc = simstd::AllocatorHeap<tst::A, heap_type>;
using EqAlloc2 = simstd::AllocatorHeap<ssize_t, heap_type>;
using test_list_type = simstd::list<tst::A, EqAlloc>;
using test_list_type2 = simstd::list<ssize_t, EqAlloc2>;

inline ssize_t get_random()
{
	return rand() % 100;
}

//inline ssize_t get_random(ssize_t l = 100)
//{
//	return rand() % l;
//}

template<typename Type>
void check_emptiness(const Type& listContainer)
{
	CRT_ASSERT(listContainer.empty());
	CRT_ASSERT(listContainer.size() == 0);
	CRT_ASSERT(simstd::begin(listContainer) == simstd::end(listContainer));
	CRT_ASSERT(simstd::cbegin(listContainer) == simstd::cend(listContainer));
	CRT_ASSERT(listContainer.begin() == listContainer.end());
	CRT_ASSERT(listContainer.cbegin() == listContainer.cend());
	CRT_ASSERT(listContainer.rbegin() == listContainer.rend());
	CRT_ASSERT(listContainer.crbegin() == listContainer.crend());
}

template<typename Type>
void check_size(const Type& listContainer, size_t size)
{
	CRT_ASSERT(!listContainer.empty());
	CRT_ASSERT(listContainer.size() == size);
	CRT_ASSERT(simstd::next(simstd::begin(listContainer), size) == simstd::end(listContainer));
	CRT_ASSERT(simstd::next(simstd::cbegin(listContainer), size) == simstd::cend(listContainer));
	CRT_ASSERT(simstd::next(listContainer.begin(), size) == listContainer.end());
	CRT_ASSERT(simstd::next(listContainer.cbegin(), size) == listContainer.cend());
	CRT_ASSERT(simstd::next(listContainer.rbegin(), size) == listContainer.rend());
	CRT_ASSERT(simstd::next(listContainer.crbegin(), size) == listContainer.crend());
}

template<typename Type>
void print_container(const char* name, const Type& listContainer)
{
	using namespace simstd;
	TestFuncFormat("%s: size(): %Id (", name, listContainer.size());
	for (auto it = begin(listContainer); it != end(listContainer); ++it) {
		TestFuncFormat(" %Id", it->val());
	}
	TestFuncFormat(")\n");
}

template<>
void print_container(const char* name, const test_list_type2& listContainer)
{
	using namespace simstd;
	TestFuncFormat("%s: size(): %Id (", name, listContainer.size());
	for (auto it = begin(listContainer); it != end(listContainer); ++it) {
		TestFuncFormat(" %Id", *it);
	}
	TestFuncFormat(")\n");
}

ssize_t tst::_list()
{
	TestFuncPlace();
	srand(static_cast<unsigned>(time(0)));

	heap_type::init();
	{
		TestFuncPlace();
		A ma1[5];
		A ma2[4];

		{
			TestFuncFormat("\n\n\n");
			TestFuncPlace();
			test_list_type list1;
			check_emptiness(list1);
			print_container("list1", list1);

			EqAlloc allocator;
			test_list_type list2(allocator);
			check_emptiness(list2);
			print_container("list2", list2);

			list1.swap(list2);
			list1.clear();
			check_emptiness(list1);
			list2.clear();
			check_emptiness(list2);
		}

		{
			TestFuncFormat("\n\n\n");
			TestFuncPlace();
			test_list_type list1(9, A(9));
			check_size(list1, 9);
			print_container("list1", list1);

			TestFuncPlace();
			EqAlloc allocator;
			test_list_type list2(8, A(8), allocator);
			check_size(list2, 8);
			print_container("list2", list2);

			TestFuncPlace();
			test_list_type list3(list2);
			check_size(list3, 8);
			print_container("list3", list3);

			TestFuncPlace();
			test_list_type list4(list1, allocator);
			check_size(list4, 9);
			print_container("list4", list4);

			TestFuncPlace();
			list4.clear();
			check_emptiness(list4);
			list3.clear();
			check_emptiness(list3);
			list2.clear();
			check_emptiness(list2);
			list1.clear();
			check_emptiness(list1);
		}

		{
			TestFuncFormat("\n\n\n");
			TestFuncPlace();
			test_list_type list1(static_cast<test_list_type::size_type>(7));
			check_size(list1, 7);
			print_container("list1", list1);

			TestFuncPlace();
			EqAlloc allocator;
			test_list_type list2(static_cast<test_list_type::size_type>(6), allocator);
			check_size(list2, 6);
			print_container("list2", list2);

			TestFuncPlace();
			test_list_type list3(simstd::move(list2));
			check_size(list3, 6);
			print_container("list3", list3);

			TestFuncPlace();
			test_list_type list4(simstd::move(list1), allocator);
			check_size(list4, 7);
			print_container("list4", list4);

			TestFuncPlace();
			list4.clear();
			check_emptiness(list4);
			list3.clear();
			check_emptiness(list3);
			check_emptiness(list2);
			check_emptiness(list1);
		}

		{
			TestFuncFormat("\n\n\n");
			TestFuncPlace();
			test_list_type list1(simstd::begin(ma1), simstd::end(ma1));
			check_size(list1, lengthof(ma1));
			print_container("list1", list1);

			TestFuncPlace();
			EqAlloc allocator;
			test_list_type list2(simstd::begin(ma2), simstd::end(ma2), allocator);
			check_size(list2, lengthof(ma2));
			print_container("list2", list2);

			TestFuncPlace();
			test_list_type list3(3, A(3));
			print_container("list3", list3);

			TestFuncPlace();
			list1.swap(list2);
			check_size(list1, lengthof(ma2));
			check_size(list2, lengthof(ma1));
			print_container("list1", list1);
			print_container("list2", list2);

			TestFuncPlace();
			list1 = list2;
			check_size(list1, list2.size());
			print_container("list1", list1);
			print_container("list2", list2);

			TestFuncPlace();
			auto list3size = list3.size();
			list2 = simstd::move(list3);
			check_size(list2, list3size);
			check_emptiness(list3);
			print_container("list2", list2);
			print_container("list3", list3);

			TestFuncPlace();
			list2.clear();
			check_emptiness(list2);
			list1.clear();
			check_emptiness(list1);
		}

		{
			TestFuncFormat("\n\n\n");
			TestFuncPlace();
			test_list_type tl1;
			test_list_type tl2;

			auto itl1b = tl1.begin();
			auto itl1e = tl1.end();
			auto itl2b = tl2.cbegin();
			auto itl2e = tl2.cend();

			if (itl1b == itl1e)
				TestFuncPlace();
			if (itl1b == itl2b)
				TestFuncPlace();
			if (itl1b == itl2e)
				TestFuncPlace();
			if (itl1e == itl1b)
				TestFuncPlace();
			if (itl1e == itl2b)
				TestFuncPlace();
			if (itl1e == itl2e)
				TestFuncPlace();
			if (itl2b == itl1b)
				TestFuncPlace();
			if (itl2b == itl1e)
				TestFuncPlace();
			if (itl2b == itl2e)
				TestFuncPlace();
			if (itl2e == itl1b)
				TestFuncPlace();
			if (itl2e == itl1e)
				TestFuncPlace();
			if (itl2e == itl2b)
				TestFuncPlace();
		}

		{
			TestFuncFormat("\n\n\n");
			TestFuncPlace();
			using std::chrono::duration_cast;
			using std::chrono::microseconds;
			using std::chrono::milliseconds;
			using std::chrono::nanoseconds;
			using std::chrono::steady_clock;
			ssize_t size = 1000;

			auto start3 = simstd::chrono::perfomance_clock::now();
			test_list_type2 list1(size);
			auto end3 = simstd::chrono::perfomance_clock::now();
			auto elapsed3 = end3 - start3;
			TestFuncPlaceFormat("size1: %10I64u, count3: %10I64u ns, count3: %10I64u mcs, count3: %6I64u ms\n", size, (uint64_t)duration_cast<nanoseconds>(elapsed3).count(), (uint64_t)duration_cast<microseconds>(elapsed3).count(), (uint64_t)duration_cast<milliseconds>(elapsed3).count());
			print_container("list1", list1);

			TestFuncPlaceFormat("generate:\n");
			start3 = simstd::chrono::perfomance_clock::now();
			simstd::generate(simstd::begin(list1), simstd::end(list1), get_random);
			end3 = simstd::chrono::perfomance_clock::now();
			elapsed3 = end3 - start3;
			TestFuncPlaceFormat("size2: %10I64u, count3: %10I64u ns, count3: %10I64u mcs, count3: %6I64u ms\n", size, (uint64_t)duration_cast<nanoseconds>(elapsed3).count(), (uint64_t)duration_cast<microseconds>(elapsed3).count(), (uint64_t)duration_cast<milliseconds>(elapsed3).count());
			print_container("list1", list1);

			TestFuncPlaceFormat("sorting:\n");
			start3 = simstd::chrono::perfomance_clock::now();
			list1.sort();
			end3 = simstd::chrono::perfomance_clock::now();
			elapsed3 = end3 - start3;
			TestFuncPlaceFormat("size3: %10I64u, count3: %10I64u ns, count3: %10I64u mcs, count3: %6I64u ms\n", size, (uint64_t)duration_cast<nanoseconds>(elapsed3).count(), (uint64_t)duration_cast<microseconds>(elapsed3).count(), (uint64_t)duration_cast<milliseconds>(elapsed3).count());
			print_container("list1", list1);
		}
//
//		A a;
//
//		test_list_type list1;
//		LogReport(L"1:size(): %Iu\n", list1.size());
//		LogReport(L"1:empty(): %d\n", list1.empty());
//
//		list1.emplace_back(1);
//		LogReport(L"1:size(): %Iu\n", list1.size());
//		LogReport(L"1:empty(): %d\n", list1.empty());
//
//		list1.emplace_back(2);
//		LogReport(L"1:size(): %Iu\n", list1.size());
//		LogReport(L"1:empty(): %d\n", list1.empty());
//
//		list1.emplace_back(3);
//		LogReport(L"1:size(): %Iu\n", list1.size());
//		LogReport(L"1:empty(): %d\n", list1.empty());
//
//		list1.reverse();
//
//		list1.pop_back();
//		LogReport(L"1:size(): %Iu\n", list1.size());
//		LogReport(L"1:empty(): %d\n", list1.empty());
//
//		for (auto it = list1.begin(); it != list1.end(); ++it)
//			LogReport(L"%d\n", it->val());
//
//		test_list_type list2(5, a);
//		LogReport(L"2:size(): %Iu\n", list2.size());
//		LogReport(L"2:empty(): %d\n", list2.empty());
//
//		test_list_type list3(10);
//		LogReport(L"3:size(): %Iu\n", list3.size());
//		LogReport(L"3:empty(): %d\n", list3.empty());
//
////		test_list_type list4(simstd::begin(ma), simstd::end(ma));
////		LogReport(L"4:size(): %Iu\n", list4.size());
////		LogReport(L"4:empty(): %d\n", list4.empty());
////
////		test_list_type list4a(list4.begin(), list4.end());
////		LogReport(L"4a:size(): %Iu\n", list4a.size());
////		LogReport(L"4a:empty(): %d\n", list4a.empty());
//
//		test_list_type list5(list2);
//		LogReport(L"5:size(): %Iu\n", list5.size());
//		LogReport(L"5:empty(): %d\n", list5.empty());
//
//		test_list_type list6(simstd::move(list3));
//		LogReport(L"6:size(): %Iu\n", list6.size());
//		LogReport(L"6:empty(): %d\n", list6.empty());
//		LogReport(L"3:size(): %Iu\n", list3.size());
//		LogReport(L"3:empty(): %d\n", list3.empty());
//
//		list1.emplace_back(a);
//
//		list1.push_back(a);
//
//		list1.push_back(a + a);
//
//		list1.push_back(simstd::move(a));
//
////		test_list_type list6;
////		LogReport(L"6:empty(): %d\n", list6.empty());
////		list6.erase(list6.begin());
	}
	heap_type::destroy();

	{
		const auto stat = heap_type::get_stat();
		LogReport(L"Heap '%S' statistics:\n", heap_type::get_name());
		LogReport(L"  alloc: %I64u, %I64u\n", stat.get_allocations(), stat.get_allocations_size());
		LogReport(L"  free : %I64u, %I64u\n", stat.get_frees(), stat.get_frees_size());
		LogReport(L"  diff : %I64d\n", stat.get_allocations_size() - stat.get_frees_size());

		LogReport(L"  objects A: %Id\n", A::objects);
	}
	return 0;
}
